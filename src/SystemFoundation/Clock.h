#ifndef MIDILAR_HARDWARE_CLOCK_H
#define MIDILAR_HARDWARE_CLOCK_H

    #include <MIDILAR_BuildSettings.h>
    #include <stdint.h>

    namespace MIDILAR::SystemFoundation{
  
        class Clock{
        public:
            enum Timebase : uint32_t {
                Seconds = 1,
                Milliseconds = 1000,
                Microseconds = 1000000,
                Nanoseconds = 1000000000
            };
            using TimePoint = uint32_t;
            using HardwareCallback = TimePoint(*)();


        protected:
            HardwareCallback _callback;
            TimePoint _currentTime;
            Timebase _clockFrequency;
        
        public:
            Clock();
            Clock(HardwareCallback hc, float freq);

            void bindCallback(HardwareCallback hc);
            void unbindCallback();
            bool callbackStatus() const;

            void setFrequency(Timebase Frequency);
            Timebase getFrequency() const;

            TimePoint refresh();
            TimePoint getTime() const;
        };
    }

#endif//MIDILAR_HARDWARE_CLOCK_H
