#include <Arduino.h>
#include <MIDILAR_SystemFoundation.h>

using namespace MIDILAR::SystemFoundation;

// ===============================
// Clock Callback Function (Microseconds)
// ===============================
/**
 * @brief Returns the current system time in microseconds.
 */
Clock::TimePoint myClockCallback() {
    return micros(); // Returns time in microseconds
}

// ===============================
// Arduino Setup & Loop
// ===============================
Clock myClock(myClockCallback, Clock::Timebase::Microseconds);

void setup() {
    Serial.begin(115200);

    // Initialize Clock with callback and set frequency
    myClock.bindClock(myClockCallback);
    myClock.setFrequency(Clock::Timebase::Microseconds);

    Serial.println("Clock Initialized!");
}

void loop() {
    static uint32_t startTime = micros(); // Store start time

    // Run for exactly 5000000 microseconds (5 seconds)
    while (true) {
        uint32_t elapsed = micros() - startTime;

        // Break after 5000 microseconds
        if (elapsed >= 5000000) {
            Serial.println("End of Clock Test.");
            while (true); // Stop execution
        }

        // Print the refreshed clock value
        Serial.println(myClock.refresh());
    }
}
