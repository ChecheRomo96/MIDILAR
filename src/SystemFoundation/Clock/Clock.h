/**
 * @file MIDILAR_SystemClockClass.h
 * @brief Defines the `Clock` class for managing system timing in the MIDILAR framework.
 */

#ifndef MIDILAR_SYSTEM_CLOCK_CLASS_H
#define MIDILAR_SYSTEM_CLOCK_CLASS_H

    #include <System/BuildSettings.h>
    #include <stdint.h>

    #include <SystemFoundation/CallbackHandler/CallbackHandler.h>

    namespace MIDILAR::SystemFoundation {

        /**
         * @class Clock
         * @brief A class for managing system timing and clock-related operations.
         */
            class Clock {
            public:
                /**
                 * @enum Timebase
                 * @brief Enumeration of timebase units.
                 */
                enum Timebase : uint32_t {
                    Seconds = 1,           /**< Time in seconds. */
                    Milliseconds = 1000,   /**< Time in milliseconds. */
                    Microseconds = 1000000,/**< Time in microseconds. */
                    Nanoseconds = 1000000000 /**< Time in nanoseconds. */
                };

                /**
                 * @typedef TimePoint
                 * @brief Alias for representing time points in the clock.
                 */
                using TimePoint = uint32_t;

                using ClockCallback = CallbackHandler<Clock::TimePoint, void>;
                using SetupCallback = CallbackHandler<void, Clock::Timebase>;



            protected:
                TimePoint _currentTime;        /**< Current time as reported by the clock. */
                Timebase _clockFrequency;      /**< The frequency of the clock. */

                CallbackHandler<TimePoint, void>  _clockPoll; /**< Pointer to the hardware callback function. */
                CallbackHandler<void, Timebase>  _clockSetup; /**< Pointer to the hardware callback function. */

            public:
                /**
                 * @brief Default constructor for the `Clock` class.
                 */
                Clock();

                /**
                 * @brief Constructs a `Clock` with a hardware callback and frequency.
                 * @param hc Pointer to the hardware callback function.
                 * @param freq The clock frequency in Hz.
                 */
                Clock(ClockCallback::CallbackReferenceType hc, float freq);

                /**
                 * @brief Binds a hardware callback to the clock.
                 * @param hc Pointer to the hardware callback function.
                 */
                void bindClock(ClockCallback::CallbackReferenceType hc);

                /**
                 * @brief Unbinds the hardware callback from the clock.
                 */
                void unbindClock();

                /**
                 * @brief Checks if a hardware callback is currently bound.
                 * @return True if a callback is bound, otherwise false.
                 */
                bool clockStatus() const;

                /**
                 * @brief Binds a hardware callback to the clock.
                 * @param hc Pointer to the hardware callback function.
                 */
                void bindSetup(SetupCallback::CallbackReferenceType hc);

                /**
                 * @brief Unbinds the hardware callback from the clock.
                 */
                void unbindSetup();

                /**
                 * @brief Checks if a hardware callback is currently bound.
                 * @return True if a callback is bound, otherwise false.
                 */
                bool setupStatus() const;

                /**
                 * @brief Sets the frequency of the clock.
                 * @param Frequency The new frequency to set.
                 */
                void setFrequency(Timebase Frequency);
                
                /**
                 * @brief Sets the frequency of the clock.
                 * @param Period The new period to set.
                 */
                void setPeriod(float Period);

                /**
                 * @brief Retrieves the current frequency of the clock.
                 * @return The clock's frequency.
                 */
                Timebase getFrequency() const;

                /**
                 * @brief Refreshes the current time using the hardware callback.
                 * @return The refreshed current time.
                 */
                TimePoint refresh();

                /**
                 * @brief Retrieves the current time as reported by the clock.
                 * @return The current time.
                 */
                TimePoint getTime() const;
            };

    } // namespace MIDILAR::SystemFoundation

#endif // MIDILAR_SYSTEM_CLOCK_CLASS_H
