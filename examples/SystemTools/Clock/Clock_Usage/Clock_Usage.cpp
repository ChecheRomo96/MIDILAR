#include <iostream>
#include <chrono>
#include <cstdint>
#include <SystemTools/Clock/Clock.h>

using namespace MIDILAR::SystemTools;

// Define the clock callback function
Clock::TimePoint myClockCallback() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()); // Subtract start time
    return static_cast<uint32_t>(duration.count());
}

int main() {
    // Initialize the clock with the callback and set timebase to milliseconds
    Clock myClock(myClockCallback, Clock::Timebase::Milliseconds);

    // Link the hardware API to the callback
    myClock.bindCallback(myClockCallback);
    myClock.setFrequency(Clock::Timebase::Milliseconds);

    // Get the start time
    auto start = std::chrono::high_resolution_clock::now();

    // Loop for 5000 microseconds (~5 milliseconds)
    while (true) {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - start);

        // Break after 5000 microseconds
        if (elapsed.count() >= 5000) {
            break;
        }

        // Print the refreshed clock value
        std::cout << myClock.refresh() << std::endl;
    }

    return 0;
}