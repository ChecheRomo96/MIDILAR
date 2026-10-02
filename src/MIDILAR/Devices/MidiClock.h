#ifndef MIDILAR_DEVICES_MIDI_CLOCK_H
#define MIDILAR_DEVICES_MIDI_CLOCK_H

#include <stdint.h>

#include <Foundation/Time/Clock.h>
#include <MIDILAR/Protocol/Channel.h>
#include <MIDILAR/Protocol/Packet.h>

#include "Device.h"

namespace MIDILAR::Devices {

/** @brief MIDI clocks per quarter note. @ingroup MIDILAR_Devices */
constexpr uint8_t ClocksPerQuarterNote = 24;

/** @brief MIDI clocks per song-position beat (a sixteenth note). @ingroup MIDILAR_Devices */
constexpr uint8_t ClocksPerSongPositionBeat = 6;

/**
 * @brief Sends MIDI clock at a tempo, timed by a `Foundation::Time::Clock`
 * (SPEC-CLK-1..3).
 * @ingroup MIDILAR_Devices
 *
 * Call `Update()` often (every loop iteration); it emits every timing clock
 * that is due. Clocks are scheduled from the previous due time, not from the
 * call time, so late calls do not make the tempo drift. Timing clocks run
 * while stopped too, so receivers keep the tempo; `Position()` only advances
 * while running.
 *
 * The tempo is in hundredths of a BPM (`12000` is 120 BPM).
 */
class ClockGenerator : public Device {
public:
    /** @brief Most clocks one `Update()` emits after a stall; later ones are skipped. */
    static constexpr uint8_t MaxCatchUp = ClocksPerQuarterNote;

    /** @brief Uses `clock` for timing and sends on `group` (default: group 1). */
    explicit ClockGenerator(const Foundation::Time::Clock& clock,
                            Protocol::Group group = Protocol::Group::FromWire(0)) noexcept
        : _clock(clock), _group(group.IsValid() ? group : Protocol::Group::FromWire(0)) {
        SetTempo(12000);
    }

    /** @brief Sets the tempo in hundredths of a BPM, clamped to `[100, 99999]`. */
    void SetTempo(uint32_t centiBpm) noexcept {
        _tempo = centiBpm < 100 ? 100 : (centiBpm > 99999 ? 99999 : centiBpm);
        // Ticks per clock = frequency * 60 / (bpm * 24) = num * 250 / (den * centiBpm).
        const Foundation::Time::Frequency frequency = _clock.GetFrequency();
        _stepNumerator = static_cast<uint64_t>(frequency.Numerator()) * 250u;
        _stepDenominator = static_cast<uint64_t>(frequency.Denominator()) * _tempo;
        _remainder = 0;
    }

    /** @brief Returns the tempo in hundredths of a BPM. */
    uint32_t Tempo() const noexcept { return _tempo; }

    /** @brief Sends Start and plays from the beginning. */
    void Start() {
        _running = true;
        _position = 0;
        Emit(Protocol::Packet::System(_group, Protocol::SystemStatus::Start));
    }

    /** @brief Sends Continue and plays from `Position()`. */
    void Continue() {
        _running = true;
        Emit(Protocol::Packet::System(_group, Protocol::SystemStatus::Continue));
    }

    /** @brief Sends Stop; `Position()` keeps its value. */
    void Stop() {
        _running = false;
        Emit(Protocol::Packet::System(_group, Protocol::SystemStatus::Stop));
    }

    /**
     * @brief Moves to `beats` sixteenth notes (`0-16383`) and sends Song
     * Position. Ignored while running, as MIDI requires.
     */
    void SetSongPosition(uint16_t beats) {
        if (_running || beats > 16383) {
            return;
        }
        _position = static_cast<uint32_t>(beats) * ClocksPerSongPositionBeat;
        Emit(Protocol::Packet::SongPosition(_group, beats));
    }

    /** @brief Emits the timing clocks that are due. */
    void Update() {
        const uint32_t now = _clock.Now().Ticks();
        if (!_started) {
            _started = true;
            _next = now;
        }
        uint8_t sent = 0;
        while (static_cast<int32_t>(now - _next) >= 0) {
            if (sent == MaxCatchUp) {
                // Too far behind: drop the backlog and restart the schedule.
                _next = now;
                _remainder = 0;
                Advance();
                return;
            }
            Emit(Protocol::Packet::System(_group, Protocol::SystemStatus::TimingClock));
            if (_running) {
                ++_position;
            }
            ++sent;
            Advance();
        }
    }

    /** @brief Returns `true` between Start/Continue and Stop. */
    bool IsRunning() const noexcept { return _running; }

    /** @brief Returns the MIDI clocks played since the song start. */
    uint32_t Position() const noexcept { return _position; }

private:
    void Advance() noexcept {
        const uint64_t total = _stepNumerator + _remainder;
        const uint64_t step = _stepDenominator == 0 ? 1 : total / _stepDenominator;
        _remainder = _stepDenominator == 0 ? 0 : total % _stepDenominator;
        _next += static_cast<uint32_t>(step == 0 ? 1 : step);
    }

    const Foundation::Time::Clock& _clock;
    Protocol::Group _group;
    uint32_t _tempo = 0;
    uint64_t _stepNumerator = 0;
    uint64_t _stepDenominator = 0;
    uint64_t _remainder = 0;
    uint32_t _next = 0;
    uint32_t _position = 0;
    bool _started = false;
    bool _running = false;
};

/**
 * @brief Follows incoming MIDI clock: transport state, song position and
 * tempo (SPEC-CLK-1..3).
 * @ingroup MIDILAR_Devices
 *
 * Every packet passes through unchanged. Start resets `Position()` to zero,
 * Continue resumes, Stop pauses, Song Position moves it while stopped, and
 * each timing clock advances it while running. The tempo is measured from
 * the time between timing clocks, smoothed over about eight clocks.
 */
class ClockReceiver : public Device {
public:
    /** @brief Uses `clock` to time incoming timing clocks. */
    explicit ClockReceiver(const Foundation::Time::Clock& clock) noexcept : _clock(clock) {}

    /** @brief Updates the state from `packet` and emits it. */
    void Process(const Protocol::Packet& packet) {
        if (packet.Type() == Protocol::MessageType::System) {
            switch (packet.SystemStatus()) {
            case Protocol::SystemStatus::TimingClock:
                Tick();
                break;
            case Protocol::SystemStatus::Start:
                _running = true;
                _position = 0;
                break;
            case Protocol::SystemStatus::Continue:
                _running = true;
                break;
            case Protocol::SystemStatus::Stop:
                _running = false;
                break;
            case Protocol::SystemStatus::SongPosition:
                if (!_running) {
                    _position = static_cast<uint32_t>(packet.SongPosition()) * ClocksPerSongPositionBeat;
                }
                break;
            default:
                break;
            }
        }
        Emit(packet);
    }

    /** @brief Returns `true` between Start/Continue and Stop. */
    bool IsRunning() const noexcept { return _running; }

    /** @brief Returns the MIDI clocks played since the song start. */
    uint32_t Position() const noexcept { return _position; }

    /** @brief Returns the measured tempo in hundredths of a BPM, or `0` before two clocks. */
    uint32_t Tempo() const noexcept {
        if (_interval == 0) {
            return 0;
        }
        const Foundation::Time::Frequency frequency = _clock.GetFrequency();
        // centiBpm = frequency * 60 * 100 / (24 * interval), interval in ticks * 8.
        const uint64_t numerator = static_cast<uint64_t>(frequency.Numerator()) * 250u * 8u;
        const uint64_t denominator = static_cast<uint64_t>(frequency.Denominator()) * _interval;
        return denominator == 0 ? 0 : static_cast<uint32_t>((numerator + denominator / 2) / denominator);
    }

private:
    void Tick() {
        const uint32_t now = _clock.Now().Ticks();
        if (_hasLast) {
            const uint32_t interval = now - _last;
            // Exponential average kept as 8 x interval for precision.
            _interval = _interval == 0 ? interval * 8u : _interval - _interval / 8u + interval;
        }
        _last = now;
        _hasLast = true;
        if (_running) {
            ++_position;
        }
    }

    const Foundation::Time::Clock& _clock;
    uint32_t _last = 0;
    uint32_t _interval = 0;
    uint32_t _position = 0;
    bool _hasLast = false;
    bool _running = false;
};

} // namespace MIDILAR::Devices

#endif // MIDILAR_DEVICES_MIDI_CLOCK_H
