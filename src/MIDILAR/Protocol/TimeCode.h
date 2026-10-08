#ifndef MIDILAR_PROTOCOL_TIME_CODE_H
#define MIDILAR_PROTOCOL_TIME_CODE_H

#include <stdint.h>

namespace MIDILAR::Protocol {

/** @brief MIDI Time Code frame rate, as encoded in MTC. @ingroup MIDILAR_Protocol */
enum class TimeCodeRate : uint8_t {
    Fps24 = 0,       ///< 24 frames per second (film).
    Fps25 = 1,       ///< 25 frames per second (PAL).
    Fps29_97Drop = 2, ///< 30000/1001 frames per second, drop-frame numbering (NTSC).
    Fps30 = 3        ///< 30 frames per second.
};

/**
 * @brief An SMPTE time `hh:mm:ss:ff` at an MTC frame rate (SPEC-MTC-1).
 * @ingroup MIDILAR_Protocol
 *
 * Hours run `0-23`. At 29.97 drop-frame, frame numbers 0 and 1 do not exist
 * at the start of each minute except minutes 0, 10, 20, 30, 40 and 50, so
 * the numbering keeps up with real time. Invalid input produces the invalid
 * time code (SPEC-RT-3).
 */
class TimeCode {
    static constexpr uint8_t InvalidHours = 0xFF;

    uint8_t _hours;
    uint8_t _minutes;
    uint8_t _seconds;
    uint8_t _frames;
    TimeCodeRate _rate;

    constexpr TimeCode(uint8_t h, uint8_t m, uint8_t s, uint8_t f, TimeCodeRate rate) noexcept
        : _hours(h), _minutes(m), _seconds(s), _frames(f), _rate(rate) {}

    static constexpr bool IsDropped(int32_t m, int32_t s, int32_t f, TimeCodeRate rate) noexcept {
        return rate == TimeCodeRate::Fps29_97Drop && s == 0 && f < 2 && m % 10 != 0;
    }

public:
    /** @brief Returns the frames per second of `rate` (30 for 29.97 drop-frame). */
    static constexpr uint8_t FramesPerSecond(TimeCodeRate rate) noexcept {
        return rate == TimeCodeRate::Fps24 ? 24 : (rate == TimeCodeRate::Fps25 ? 25 : 30);
    }

    /** @brief Creates the invalid time code. */
    constexpr TimeCode() noexcept : TimeCode(InvalidHours, 0, 0, 0, TimeCodeRate::Fps30) {}

    /** @brief Returns the invalid time code. */
    static constexpr TimeCode Invalid() noexcept { return TimeCode(); }

    /** @brief Creates `h:m:s:f` at `rate`, or the invalid time code. */
    static constexpr TimeCode From(int32_t h, int32_t m, int32_t s, int32_t f, TimeCodeRate rate) noexcept {
        return static_cast<uint8_t>(rate) <= 3 && h >= 0 && h < 24 && m >= 0 && m < 60 && s >= 0 && s < 60 &&
                       f >= 0 && f < FramesPerSecond(rate) && !IsDropped(m, s, f, rate)
            ? TimeCode(static_cast<uint8_t>(h), static_cast<uint8_t>(m), static_cast<uint8_t>(s),
                       static_cast<uint8_t>(f), rate)
            : TimeCode();
    }

    /** @brief Returns `true` unless this is the invalid time code. */
    constexpr bool IsValid() const noexcept { return _hours != InvalidHours; }

    /** @brief Returns the hours, `0-23`. */
    constexpr uint8_t Hours() const noexcept { return _hours; }

    /** @brief Returns the minutes, `0-59`. */
    constexpr uint8_t Minutes() const noexcept { return _minutes; }

    /** @brief Returns the seconds, `0-59`. */
    constexpr uint8_t Seconds() const noexcept { return _seconds; }

    /** @brief Returns the frame number. */
    constexpr uint8_t Frames() const noexcept { return _frames; }

    /** @brief Returns the frame rate. */
    constexpr TimeCodeRate Rate() const noexcept { return _rate; }

    /** @brief Returns the next frame, wrapping from 23:59:59 to 00:00:00. */
    FOUNDATION_CONSTEXPR14 TimeCode Next() const noexcept {
        if (!IsValid()) {
            return TimeCode();
        }
        int32_t h = _hours;
        int32_t m = _minutes;
        int32_t s = _seconds;
        int32_t f = _frames + 1;
        if (f >= FramesPerSecond(_rate)) {
            f = 0;
            ++s;
        }
        if (s == 60) {
            s = 0;
            ++m;
        }
        if (m == 60) {
            m = 0;
            ++h;
        }
        if (h == 24) {
            h = 0;
        }
        if (IsDropped(m, s, f, _rate)) {
            f = 2;
        }
        return TimeCode(static_cast<uint8_t>(h), static_cast<uint8_t>(m), static_cast<uint8_t>(s),
                        static_cast<uint8_t>(f), _rate);
    }

    /**
     * @brief Returns the data byte of quarter-frame message `piece` (`0-7`):
     * the piece number in the high nibble and four bits of the time below.
     */
    constexpr uint8_t QuarterFrame(uint8_t piece) const noexcept {
        return static_cast<uint8_t>(((piece & 7u) << 4) | (QuarterFrameNibble(piece & 7u) & 0x0Fu));
    }

    /**
     * @brief Rebuilds a time code from the eight quarter-frame data nibbles,
     * indexed by piece; invalid fields give the invalid time code.
     */
    static constexpr TimeCode FromQuarterFrames(const uint8_t (&nibbles)[8]) noexcept {
        return From(Join(nibbles[7] & 1u, nibbles[6]), Join(nibbles[5] & 3u, nibbles[4]),
                    Join(nibbles[3] & 3u, nibbles[2]), Join(nibbles[1] & 1u, nibbles[0]),
                    static_cast<TimeCodeRate>((nibbles[7] >> 1) & 3u));
    }

    /** @brief Returns the full-frame hours byte: rate in bits 6-5, hours below. */
    constexpr uint8_t RateAndHours() const noexcept {
        return static_cast<uint8_t>((static_cast<uint8_t>(_rate) << 5) | _hours);
    }

    /** @brief Compares time and rate; all invalid time codes are equal. */
    friend constexpr bool operator==(const TimeCode& a, const TimeCode& b) noexcept {
        return a._hours == b._hours &&
            (!a.IsValid() || (a._minutes == b._minutes && a._seconds == b._seconds && a._frames == b._frames &&
                              a._rate == b._rate));
    }

    /** @brief Negation of `operator==`. */
    friend constexpr bool operator!=(const TimeCode& a, const TimeCode& b) noexcept { return !(a == b); }

private:
    static constexpr int32_t Join(uint32_t high, uint32_t low) noexcept {
        return static_cast<int32_t>((high << 4) | (low & 0x0Fu));
    }

    constexpr uint32_t QuarterFrameNibble(uint32_t piece) const noexcept {
        return piece == 0   ? _frames & 0x0Fu
            : piece == 1 ? static_cast<uint32_t>(_frames) >> 4
            : piece == 2 ? _seconds & 0x0Fu
            : piece == 3 ? static_cast<uint32_t>(_seconds) >> 4
            : piece == 4 ? _minutes & 0x0Fu
            : piece == 5 ? static_cast<uint32_t>(_minutes) >> 4
            : piece == 6 ? _hours & 0x0Fu
                         : (static_cast<uint32_t>(_hours) >> 4) | (static_cast<uint32_t>(_rate) << 1);
    }
};

} // namespace MIDILAR::Protocol

#endif // MIDILAR_PROTOCOL_TIME_CODE_H
