#include <Arduino.h>
#include <MIDILAR_SystemFoundation.h>

using namespace MIDILAR::SystemFoundation;

// ===============================
// Clock Callback Function
// ===============================
/**
 * @brief Returns the current system time in milliseconds.
 */
Clock::TimePoint myClockCallback() {
    return millis(); // Returns time in milliseconds
}

// ===============================
// Arduino Setup & Loop
// ===============================
Clock myClock(myClockCallback, Clock::Timebase::Milliseconds);

void setup() {
    Serial.begin(115200);

    // Initialize Clock with callback and set frequency
    myClock.bindClock(myClockCallback);
    myClock.setFrequency(Clock::Timebase::Milliseconds);

    Serial.println("Clock Initialized!");
}

void loop() {
    static uint32_t startTime = millis(); // Store start time

    // Run for exactly 5000 milliseconds (5 seconds)
    while (true) {
        uint32_t elapsed = millis() - startTime;

        // Break after 5000 milliseconds
        if (elapsed >= 5000) {
            Serial.println("End of Clock Test.");
            while (true); // Stop execution
        }

        // Print the refreshed clock value
        Serial.println(myClock.refresh());
    }
}
