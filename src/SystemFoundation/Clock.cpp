#include "Clock.h"

namespace MIDILAR::SystemFoundation {

    // Default Constructor
    Clock::Clock()
        : _callback(nullptr), _currentTime(0), _clockFrequency(Timebase::Seconds) {}

    // Parameterized Constructor
    Clock::Clock(HardwareCallback hc, float freq)
        : _callback(hc), _currentTime(0), _clockFrequency(static_cast<Timebase>(freq)) {}

    // Link a hardware API function
    void Clock::bindCallback(HardwareCallback hc) {
        _callback = hc;
    }

    // Unlink the hardware API
    void Clock::unbindCallback() {
        _callback = nullptr;
    }

    // Check if the hardware API is linked
    bool Clock::callbackStatus() const {
        return _callback != nullptr;
    }

    // Set the clock's timebase (frequency)
    void Clock::setFrequency(Timebase Frequency) {
        if (Frequency == 1.0f)
            _clockFrequency = Timebase::Seconds;
        else if (Frequency == 1000.0f)
            _clockFrequency = Timebase::Milliseconds;
        else if (Frequency == 1000000.0f)
            _clockFrequency = Timebase::Microseconds;
        else if (Frequency == 1000000000.0f)
            _clockFrequency = Timebase::Nanoseconds;
        else
            _clockFrequency = Timebase::Seconds; // Default to Seconds if invalid frequency is provided
    }

    // Get the current timebase (frequency)
    Clock::Timebase Clock::getFrequency() const {
        return static_cast<Timebase>(_clockFrequency);
    }

    // Refresh the current clock time
    Clock::TimePoint Clock::refresh() {
        if (_callback) {
            _currentTime = _callback();
        }
        return _currentTime;
    }

    // Get the current clock time
    Clock::TimePoint Clock::getTime() const {
        return _currentTime;
    }
}
