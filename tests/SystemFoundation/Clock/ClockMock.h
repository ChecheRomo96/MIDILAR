#ifndef MIDILAR_TEST_SYSTEM_CLOCK_MOCK_H
#define MIDILAR_TEST_SYSTEM_CLOCK_MOCK_H

#include <SystemFoundation/Clock/Clock.h>

namespace MIDILAR::Tests::SystemFoundation {

    class ClockMock {
    public:
        using TimePoint = MIDILAR::SystemFoundation::Clock::TimePoint;
        using Timebase  = MIDILAR::SystemFoundation::Clock::Timebase;

    private:
        TimePoint _CurrentTime;
        Timebase  _Frequency;

    public:
        ClockMock() :
            _CurrentTime(0),
            _Frequency(Timebase::Seconds) {
        }

        TimePoint GetCurrentTime() const {
            return _CurrentTime;
        }

        void SetCurrentTime(TimePoint Time) {
            _CurrentTime = Time;
        }

        void AdvanceTime(TimePoint Delta) {
            _CurrentTime += Delta;
        }

        void RewindTime(TimePoint Delta) {
            _CurrentTime -= Delta;
        }

        Timebase GetFrequency() const {
            return _Frequency;
        }

        void SetFrequency(Timebase Frequency) {
            _Frequency = Frequency;
        }

        void Reset() {
            _CurrentTime = 0;
            _Frequency = Timebase::Seconds;
        }
    };

    inline ClockMock GClockMock;

    inline MIDILAR::SystemFoundation::Clock::TimePoint ClockMockCallback() {
        return GClockMock.GetCurrentTime();
    }

    inline void ClockMockSetupCallback(MIDILAR::SystemFoundation::Clock::Timebase Frequency) {
        GClockMock.SetFrequency(Frequency);
    }

}

#endif
