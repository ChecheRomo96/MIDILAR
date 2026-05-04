#ifndef MIDILAR_TEST_SYSTEM_CLOCK_MOCK_H
#define MIDILAR_TEST_SYSTEM_CLOCK_MOCK_H

#include <SystemCore/Clock/Clock.h>

namespace MIDILAR::Tests::SystemCore {

    class ClockMock {
    public:
        using TimePoint = MIDILAR::SystemCore::Clock::TimePoint;
        using Freq  = MIDILAR::SystemCore::Clock::Freq;

    private:
        TimePoint _CurrentTime;
        Freq  _Frequency;

    public:
        ClockMock() :
            _CurrentTime(0),
            _Frequency(Freq::Seconds) {
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

        Freq GetFrequency() const {
            return _Frequency;
        }

        void SetFrequency(Freq Frequency) {
            _Frequency = Frequency;
        }

        void Reset() {
            _CurrentTime = 0;
            _Frequency = Freq::Seconds;
        }
    };

    inline ClockMock GClockMock;

    inline MIDILAR::SystemCore::Clock::TimePoint ClockMockCallback() {
        return GClockMock.GetCurrentTime();
    }

    inline void ClockMockSetupCallback(MIDILAR::SystemCore::Clock::Freq Frequency) {
        GClockMock.SetFrequency(Frequency);
    }

}

#endif
