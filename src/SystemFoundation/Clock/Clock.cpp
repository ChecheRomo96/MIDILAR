#include "Clock.h"

namespace MIDILAR::SystemFoundation {

    /**
     * @brief Default Constructor
     * Initializes the Clock with no callback, a default time of 0, and a timebase of seconds.
     */
    Clock::Clock() : _currentTime(0), _clockFrequency(Timebase::Seconds), _clockPoll(), _clockSetup() {}

    /**
     * @brief Parameterized Constructor
     * Initializes the Clock with a hardware callback and specified frequency.
     * @param hc Pointer to the hardware callback function.
     * @param freq The frequency of the clock in Hz.
     */
    Clock::Clock(ClockCallback::CallbackReferenceType hc, float freq) : Clock() {
        
        
        // Initialize the callback handler for clock polling
        _clockPoll.bind(hc);

        // Only in this case one can bypass the set te frequency
        // without a setup callback binded
        // (assuming freq is in Hz and you want to convert it)
        _clockFrequency = static_cast<Timebase>(freq);
    }

    /**
     * @brief Links a hardware API function to the Clock.
     * @param hc Pointer to the hardware callback function.
     */
    void Clock::bindClock(ClockCallback::CallbackReferenceType hc) {
        _clockPoll.bind(hc);
    }

    /**
     * @brief Unlinks the hardware API function from the Clock.
     */
    void Clock::unbindClock() {
        _clockPoll.unbind();
    }

    /**
     * @brief Checks if a hardware API function is linked to the Clock.
     * @return True if a callback is linked; false otherwise.
     */
    bool Clock::clockStatus() const {
        return _clockPoll.status();
    }

    /**
     * @brief Binds a setup callback to adjust the hardware timebase.
     * @param sc Pointer to the setup callback function.
     */
    void Clock::bindSetup(SetupCallback::CallbackReferenceType sc) {
        _clockSetup.bind(sc);
    }

    /**
     * @brief Unbinds the setup callback from the clock.
     */
    void Clock::unbindSetup() {
        _clockSetup.unbind();
    }
    
    /**
     * @brief Checks if a hardware API function is linked to the Clock.
     * @return True if a callback is linked; false otherwise.
     */
    bool Clock::setupStatus() const {
        return _clockSetup.status();
    }

    /**
     * @brief Sets the clock's timebase (frequency).
     * @param Frequency The timebase to set (Seconds, Milliseconds, etc.).
     * Defaults to Seconds if an invalid frequency is provided.
     */
    void Clock::setFrequency(Timebase frequency) {
        if(frequency>0.0f){
            if (_clockSetup.status()) {
                _clockFrequency = frequency;
                _clockSetup.invoke(frequency);
            }
        }
    }

    /**
     * @brief Retrieves the current clock timebase (frequency).
     * @return The current timebase of the clock.
     */
    Clock::Timebase Clock::getFrequency() const {
        return _clockFrequency;
    }

    /**
     * @brief Sets the clock period using the setup callback.
     * Updates the internal clock's period and reconfigures the hardware if applicable.
     * @param period The new period for the clock in nanoseconds.
     */
    void Clock::setPeriod(float Period) {
        if (Period > 0) {
            // Convert period to frequency and update Timebase
            _clockFrequency = static_cast<Timebase>(1.0f / Period);
            if (_clockSetup.status()) {
                _clockSetup.invoke(_clockFrequency); // Call hardware setup if bound
            }
        }
    }
  
    /**
     * @brief Refreshes the current clock time using the hardware callback.
     * If no callback is linked, the time remains unchanged.
     * @return The updated current time.
     */
    Clock::TimePoint Clock::refresh() {
        if (_clockPoll.status()) {
            _currentTime = _clockPoll.invoke(); // Assuming callback accepts null as argument
        }
        return _currentTime;
    }

    /**
     * @brief Retrieves the current clock time.
     * @return The last updated time from the clock.
     */
    Clock::TimePoint Clock::getTime() const {
        return _currentTime;
    }

} // namespace MIDILAR::SystemFoundation
