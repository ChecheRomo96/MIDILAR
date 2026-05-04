#ifndef MIDILAR_SYSTEM_CLOCK_CONTROLLER_H
#define MIDILAR_SYSTEM_CLOCK_CONTROLLER_H

#include <MIDILAR_BuildSettings.h>

#include <SystemCore/CallbackHandler/CallbackHandler.h>
#include "ClockTypes.h"

namespace MIDILAR::SystemCore::Clock {

    class Controller {
    public:
        using ClockCallback = CallbackHandler<Tick, void>;
        using SetupCallback = CallbackHandler<void, Freq>;

    protected:
        Tick _currentTick;
        Freq _frequency;

        ClockCallback _clockPoll;
        SetupCallback _clockSetup;

    public:
        Controller()
            : _currentTick(0),
              _frequency(Freq::Hz),
              _clockPoll(),
              _clockSetup() {
        }

        Controller(ClockCallback::CallbackType clockCallback, Freq frequency)
            : _currentTick(0),
              _frequency(frequency),
              _clockPoll(),
              _clockSetup() {
            _clockPoll.bind(clockCallback);
        }

        void BindClock(ClockCallback::CallbackType callback) {
            _clockPoll.bind(callback);
        }

        void UnbindClock() {
            _clockPoll.unbind();
        }

        bool ClockStatus() const {
            return _clockPoll.status();
        }

        void BindSetup(SetupCallback::CallbackType callback) {
            _clockSetup.bind(callback);
        }

        void UnbindSetup() {
            _clockSetup.unbind();
        }

        bool SetupStatus() const {
            return _clockSetup.status();
        }

        void SetFrequency(Freq frequency) {
            _frequency = frequency;

            if (_clockSetup.status()) {
                _clockSetup.invoke(frequency);
            }
        }

        Freq GetFrequency() const {
            return _frequency;
        }

        uint32_t GetFrequencyHz() const {
            return ToHz(_frequency);
        }

        Tick Now() {
            if (_clockPoll.status()) {
                _currentTick = _clockPoll.invoke();
            }

            return _currentTick;
        }

        Tick GetTick() const {
            return _currentTick;
        }

        TimePoint NowTimePoint() {
            return TimePoint{ Now(), _frequency };
        }

        TimePoint GetTimePoint() const {
            return TimePoint{ _currentTick, _frequency };
        }
    };

}

#endif // MIDILAR_SYSTEM_CLOCK_CONTROLLER_H