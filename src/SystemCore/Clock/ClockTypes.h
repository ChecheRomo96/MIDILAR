#ifndef MIDILAR_SYSTEM_CLOCK_TYPES_H
#define MIDILAR_SYSTEM_CLOCK_TYPES_H

#include <MIDILAR_BuildSettings.h>
#include <stdint.h>

namespace MIDILAR::SystemCore::Clock {

    enum class Freq : uint32_t {
        Hz  = 1UL,
        KHz = 1000UL,
        MHz = 1000000UL,
        GHz = 1000000000UL
    };

    using Tick = uint32_t;

    constexpr Freq operator*(uint32_t lhs, Freq rhs) {
        return static_cast<Freq>(
            lhs * static_cast<uint32_t>(rhs)
        );
    }

    constexpr Freq operator*(Freq lhs, uint32_t rhs) {
        return static_cast<Freq>(
            static_cast<uint32_t>(lhs) * rhs
        );
    }

    constexpr uint32_t ToHz(Freq freq) {
        return static_cast<uint32_t>(freq);
    }

    struct Duration {
        float seconds;

        constexpr Duration()
            : seconds(0.0f) {
        }

        constexpr explicit Duration(float valueSeconds)
            : seconds(valueSeconds) {
        }

        float ToSeconds() const {
            return seconds;
        }

        float ToMilliseconds() const {
            return seconds * 1000.0f;
        }

        float ToMicroseconds() const {
            return seconds * 1000000.0f;
        }

        float ToMinutes() const {
            return seconds / 60.0f;
        }

        float ToHours() const {
            return seconds / 3600.0f;
        }
    };

    struct TimePoint {
        Tick ticks;
        Freq frequency;

        constexpr TimePoint()
            : ticks(0),
              frequency(Freq::Hz) {
        }

        constexpr TimePoint(Tick tickValue, Freq freq)
            : ticks(tickValue),
              frequency(freq) {
        }

        Tick RawTicks() const {
            return ticks;
        }

        Freq GetFrequency() const {
            return frequency;
        }

        uint32_t GetFrequencyHz() const {
            return ToHz(frequency);
        }

        float ToSeconds() const {
            return static_cast<float>(ticks) /
                   static_cast<float>(ToHz(frequency));
        }

        float ToMilliseconds() const {
            return ToSeconds() * 1000.0f;
        }

        float ToMicroseconds() const {
            return ToSeconds() * 1000000.0f;
        }
    };

    constexpr Duration Seconds(float value) {
        return Duration(value);
    }

    constexpr Duration Milliseconds(float value) {
        return Duration(value / 1000.0f);
    }

    constexpr Duration Microseconds(float value) {
        return Duration(value / 1000000.0f);
    }

    constexpr Duration Minutes(float value) {
        return Duration(value * 60.0f);
    }

    constexpr Duration Hours(float value) {
        return Duration(value * 3600.0f);
    }

    inline Duration operator-(TimePoint lhs, TimePoint rhs) {
        return Duration(lhs.ToSeconds() - rhs.ToSeconds());
    }

}

#endif // MIDILAR_SYSTEM_CLOCK_TYPES_H