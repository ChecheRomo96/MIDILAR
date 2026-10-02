#ifndef MIDILAR_DEVICES_SEQUENCER_H
#define MIDILAR_DEVICES_SEQUENCER_H

#include <stdint.h>

#include <MIDILAR/Protocol/Channel.h>
#include <MIDILAR/Protocol/Packet.h>
#include <MIDILAR/Protocol/Values.h>

#include "Device.h"
#include "MidiClock.h"

namespace MIDILAR::Devices {

/** @brief Most steps a sequencer pattern holds. @ingroup MIDILAR_Devices */
constexpr uint8_t MaxSteps = 64;

/**
 * @brief Spreads `pulses` onsets as evenly as possible over `steps` steps
 * (a Euclidean rhythm, SPEC-SEQ-1).
 * @ingroup MIDILAR_Devices
 *
 * Bit `i` of the result is step `i`. The pattern starts on an onset and
 * equals Bjorklund's up to rotation; `rotation` then shifts it later by that
 * many steps. `E(3, 8)` is `x..x..x.`. `steps` above 64 gives `0`; `pulses`
 * above `steps` is clamped.
 */
constexpr uint64_t EuclideanPattern(uint8_t pulses, uint8_t steps, uint8_t rotation = 0) noexcept {
    if (steps == 0 || steps > MaxSteps) {
        return 0;
    }
    const uint32_t onsets = pulses > steps ? steps : pulses;
    uint64_t pattern = 0;
    for (uint32_t i = 0; i < steps; ++i) {
        if ((i * onsets) % steps < onsets) {
            pattern |= static_cast<uint64_t>(1) << ((i + rotation) % steps);
        }
    }
    return pattern;
}

/**
 * @brief Plays one note on the steps of a pattern, driven by MIDI clock
 * (SPEC-SEQ-2).
 * @ingroup MIDILAR_Devices
 *
 * Bind it after a `ClockGenerator` or `ClockReceiver`. Every packet passes
 * through; while the transport runs, each timing clock advances the
 * sequencer, and every `ClocksPerStep()` clocks (default 6, a sixteenth
 * note) a step starts. Step `n` of the pattern plays a MIDI 2.0 Note On
 * when its bit is set, and the note is released after `Gate()` clocks or at
 * the next onset, whichever comes first. Stop releases the sounding note, so
 * notes never hang. Song Position moves the sequencer while stopped.
 */
class StepSequencer : public Device {
public:
    /** @brief Sets the pattern (bit `i` is step `i`) and its length, `1-64` steps. */
    void SetPattern(uint64_t pattern, uint8_t steps) noexcept {
        _steps = steps == 0 ? 1 : (steps > MaxSteps ? MaxSteps : steps);
        _pattern = _steps == MaxSteps ? pattern : pattern & ((static_cast<uint64_t>(1) << _steps) - 1);
    }

    /** @brief Returns the pattern. */
    uint64_t Pattern() const noexcept { return _pattern; }

    /** @brief Returns the pattern length in steps. */
    uint8_t Steps() const noexcept { return _steps; }

    /** @brief Sets the note, velocity and address played; invalid values keep the previous ones. */
    void SetNote(Protocol::NoteNumber note, Protocol::Velocity velocity = Protocol::Velocity::FromMidi1(100)) noexcept {
        if (note.IsValid()) {
            _note = note;
        }
        _velocity = velocity;
    }

    /** @brief Sets the group and channel the notes are sent on. */
    void SetAddress(Protocol::Group group, Protocol::Channel channel) noexcept {
        if (group.IsValid() && channel.IsValid()) {
            _group = group;
            _channel = channel;
        }
    }

    /** @brief Sets the MIDI clocks per step, `1-96` (6 is a sixteenth note, 24 a quarter note). */
    void SetClocksPerStep(uint8_t clocks) noexcept { _clocksPerStep = clocks == 0 ? 1 : (clocks > 96 ? 96 : clocks); }

    /** @brief Returns the MIDI clocks per step. */
    uint8_t ClocksPerStep() const noexcept { return _clocksPerStep; }

    /** @brief Sets how many clocks a note sounds, at least 1. */
    void SetGate(uint8_t clocks) noexcept { _gate = clocks == 0 ? 1 : clocks; }

    /** @brief Returns how many clocks a note sounds. */
    uint8_t Gate() const noexcept { return _gate; }

    /** @brief Returns `true` between Start/Continue and Stop. */
    bool IsRunning() const noexcept { return _running; }

    /** @brief Returns the step that played last, or will play next while stopped. */
    uint8_t CurrentStep() const noexcept {
        return static_cast<uint8_t>((_clock / _clocksPerStep) % _steps);
    }

    /** @brief Emits `packet`, then the notes it triggers. */
    void Process(const Protocol::Packet& packet) {
        Emit(packet);
        if (packet.Type() != Protocol::MessageType::System) {
            return;
        }
        switch (packet.SystemStatus()) {
        case Protocol::SystemStatus::Start:
            Release();
            _running = true;
            _clock = 0;
            break;
        case Protocol::SystemStatus::Continue:
            _running = true;
            break;
        case Protocol::SystemStatus::Stop:
            _running = false;
            Release();
            break;
        case Protocol::SystemStatus::SongPosition:
            if (!_running) {
                _clock = static_cast<uint32_t>(packet.SongPosition()) * ClocksPerSongPositionBeat;
            }
            break;
        case Protocol::SystemStatus::TimingClock:
            if (_running) {
                Tick();
            }
            break;
        default:
            break;
        }
    }

private:
    void Tick() {
        if (_sounding && --_gateLeft == 0) {
            Release();
        }
        if (_clock % _clocksPerStep == 0) {
            const uint8_t step = CurrentStep();
            if (((_pattern >> step) & 1u) != 0) {
                Release();
                _playing = _note;
                _sounding = true;
                _gateLeft = _gate;
                Emit(Protocol::Packet::Midi2NoteOn(_group, _channel, _playing, _velocity));
            }
        }
        ++_clock;
    }

    void Release() {
        if (_sounding) {
            _sounding = false;
            Emit(Protocol::Packet::Midi2NoteOff(_group, _channel, _playing, Protocol::Velocity()));
        }
    }

    uint64_t _pattern = 0;
    uint32_t _clock = 0;
    Protocol::Velocity _velocity = Protocol::Velocity::FromMidi1(100);
    Protocol::NoteNumber _note = Protocol::NoteNumber::FromValue(60);
    Protocol::NoteNumber _playing;
    Protocol::Group _group = Protocol::Group::FromWire(0);
    Protocol::Channel _channel = Protocol::Channel::FromWire(0);
    uint8_t _steps = 16;
    uint8_t _clocksPerStep = 6;
    uint8_t _gate = 3;
    uint8_t _gateLeft = 0;
    bool _running = false;
    bool _sounding = false;
};

} // namespace MIDILAR::Devices

#endif // MIDILAR_DEVICES_SEQUENCER_H
