#ifndef MIDILAR_DEVICES_MTC_H
#define MIDILAR_DEVICES_MTC_H

#include <stdint.h>

#include <Foundation/Time/Clock.h>
#include <MIDILAR/Protocol/Channel.h>
#include <MIDILAR/Protocol/Packet.h>
#include <MIDILAR/Protocol/TimeCode.h>

#include "Device.h"

namespace MIDILAR::Devices {

/**
 * @brief Sends MIDI Time Code, timed by a `Foundation::Time::Clock`
 * (SPEC-MTC-2).
 * @ingroup MIDILAR_Devices
 *
 * While running, `Update()` sends four quarter frames per frame. The eight
 * pieces of one message describe the frame in which piece 0 was sent, so a
 * full time takes two frames. Quarter frames are scheduled from the previous
 * due time with exact rational arithmetic (29.97 is 30000/1001), so the time
 * code does not drift. `Locate()` sends a full-frame message.
 */
class MtcGenerator : public Device {
public:
    /** @brief Most quarter frames one `Update()` emits after a stall; the backlog is dropped. */
    static constexpr uint8_t MaxCatchUp = 8;

    /** @brief Uses `clock` for timing and sends on `group` (default: group 1). */
    explicit MtcGenerator(const Foundation::Time::Clock& clock,
                          Protocol::Group group = Protocol::Group::FromWire(0)) noexcept
        : _clock(clock), _group(group.IsValid() ? group : Protocol::Group::FromWire(0)) {
        Locate(Protocol::TimeCode::From(0, 0, 0, 0, Protocol::TimeCodeRate::Fps30), false);
    }

    /**
     * @brief Moves to `time` (invalid times are ignored) and, when `announce`
     * is `true`, sends it as a full-frame message. Quarter frames restart at
     * piece 0.
     */
    void Locate(Protocol::TimeCode time, bool announce = true) {
        if (!time.IsValid()) {
            return;
        }
        _time = time;
        _piece = 0;
        // Ticks per quarter frame = frequency / (fps * 4).
        const Foundation::Time::Frequency frequency = _clock.GetFrequency();
        const bool ntsc = time.Rate() == Protocol::TimeCodeRate::Fps29_97Drop;
        _stepNumerator = static_cast<uint64_t>(frequency.Numerator()) * (ntsc ? 1001u : 1u);
        _stepDenominator = static_cast<uint64_t>(frequency.Denominator()) *
            (ntsc ? 30000u : Protocol::TimeCode::FramesPerSecond(time.Rate())) * 4u;
        _remainder = 0;
        if (announce) {
            SendFullFrame();
        }
    }

    /** @brief Starts sending quarter frames from the current time. */
    void Start() {
        _running = true;
        _piece = 0;
        _remainder = 0;
        _next = _clock.Now().Ticks();
    }

    /** @brief Stops sending quarter frames; the time stays where it is. */
    void Stop() noexcept { _running = false; }

    /** @brief Returns `true` while quarter frames are being sent. */
    bool IsRunning() const noexcept { return _running; }

    /** @brief Returns the current frame. */
    Protocol::TimeCode Time() const noexcept { return _time; }

    /** @brief Sends the current time as a full-frame System Exclusive message. */
    void SendFullFrame() {
        const uint8_t head[6] = {0x7F, 0x7F, 0x01, 0x01, _time.RateAndHours(), _time.Minutes()};
        const uint8_t tail[2] = {_time.Seconds(), _time.Frames()};
        Emit(Protocol::Packet::SysEx7(_group, Protocol::SysExStatus::Start, head, 6));
        Emit(Protocol::Packet::SysEx7(_group, Protocol::SysExStatus::End, tail, 2));
    }

    /** @brief Emits the quarter frames that are due. */
    void Update() {
        if (!_running) {
            return;
        }
        const uint32_t now = _clock.Now().Ticks();
        uint8_t sent = 0;
        while (static_cast<int32_t>(now - _next) >= 0) {
            if (sent == MaxCatchUp) {
                _next = now;
                _remainder = 0;
                Advance();
                return;
            }
            if (_piece == 0) {
                _message = _time;
            }
            Emit(Protocol::Packet::TimeCode(_group, _message.QuarterFrame(_piece)));
            _piece = static_cast<uint8_t>((_piece + 1) & 7u);
            if ((_piece & 3u) == 0) {
                _time = _time.Next();
            }
            ++sent;
            Advance();
        }
    }

private:
    void Advance() noexcept {
        const uint64_t total = _stepNumerator + _remainder;
        const uint64_t step = _stepDenominator == 0 ? 1 : total / _stepDenominator;
        _remainder = _stepDenominator == 0 ? 0 : total % _stepDenominator;
        _next += static_cast<uint32_t>(step == 0 ? 1 : step);
    }

    const Foundation::Time::Clock& _clock;
    Protocol::Group _group;
    Protocol::TimeCode _time;
    Protocol::TimeCode _message;
    uint64_t _stepNumerator = 0;
    uint64_t _stepDenominator = 0;
    uint64_t _remainder = 0;
    uint32_t _next = 0;
    uint8_t _piece = 0;
    bool _running = false;
};

/**
 * @brief Follows incoming MIDI Time Code (SPEC-MTC-3).
 * @ingroup MIDILAR_Devices
 *
 * Every packet passes through. Eight quarter frames in order (pieces 0-7)
 * set `Time()` to the frame they describe plus two, the frame being played
 * when piece 7 arrives; out-of-order pieces restart the assembly. A
 * full-frame message (`F0 7F <device> 01 01 hr mn sc fr F7`) sets `Time()`
 * directly.
 */
class MtcReceiver : public Device {
public:
    /** @brief Updates the time from `packet` and emits it. */
    void Process(const Protocol::Packet& packet) {
        if (packet.IsSystem(Protocol::SystemStatus::TimeCode)) {
            QuarterFrame(packet.TimeCode());
        } else if (packet.Type() == Protocol::MessageType::Data64) {
            SysEx(packet);
        }
        Emit(packet);
    }

    /** @brief Returns the last time received, or the invalid time code. */
    Protocol::TimeCode Time() const noexcept { return _time; }

private:
    void QuarterFrame(uint8_t data) {
        const uint8_t piece = static_cast<uint8_t>(data >> 4);
        if (piece != _expected) {
            _expected = 0;
            if (piece != 0) {
                return;
            }
        }
        _nibbles[piece] = static_cast<uint8_t>(data & 0x0Fu);
        _expected = static_cast<uint8_t>(piece + 1);
        if (piece == 7) {
            _expected = 0;
            const Protocol::TimeCode decoded = Protocol::TimeCode::FromQuarterFrames(_nibbles);
            if (decoded.IsValid()) {
                _time = decoded.Next().Next();
            }
        }
    }

    void SysEx(const Protocol::Packet& packet) {
        const Protocol::SysExStatus status = packet.SysExStatus();
        if (status == Protocol::SysExStatus::Start || status == Protocol::SysExStatus::Complete) {
            _sysExSize = 0;
        }
        for (uint8_t i = 0; i < packet.SysExSize(); ++i) {
            if (_sysExSize < FullFrameLength) {
                _sysEx[_sysExSize] = packet.SysExByte(i);
            }
            if (_sysExSize < 0xFF) {
                ++_sysExSize;
            }
        }
        if ((status == Protocol::SysExStatus::End || status == Protocol::SysExStatus::Complete) &&
            _sysExSize == FullFrameLength && _sysEx[0] == 0x7F && _sysEx[2] == 0x01 && _sysEx[3] == 0x01) {
            const Protocol::TimeCode time = Protocol::TimeCode::From(
                _sysEx[4] & 0x1F, _sysEx[5], _sysEx[6], _sysEx[7],
                static_cast<Protocol::TimeCodeRate>((_sysEx[4] >> 5) & 3u));
            if (time.IsValid()) {
                _time = time;
                _expected = 0;
            }
        }
    }

    static constexpr uint8_t FullFrameLength = 8;

    Protocol::TimeCode _time;
    uint8_t _nibbles[8] = {};
    uint8_t _expected = 0;
    uint8_t _sysEx[FullFrameLength] = {};
    uint8_t _sysExSize = 0;
};

} // namespace MIDILAR::Devices

#endif // MIDILAR_DEVICES_MTC_H
