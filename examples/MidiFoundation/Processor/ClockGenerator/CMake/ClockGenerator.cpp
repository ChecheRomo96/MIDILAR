#include <SystemFoundation/Clock/Clock.h>
#include <MidiFoundation/Processor/Processor.h>

#include <iostream>

class MidiClockProcessor : public MIDILAR::MidiFoundation::Processor {
    MIDILAR::SystemFoundation::Clock::TimePoint lastTick = 0;
private:
    uint32_t Period;
public:
    MidiClockProcessor() {
        SetCapabilities(static_cast<uint32_t>(Capabilities::MidiOut) |
                        static_cast<uint32_t>(Capabilities::InternalClock));
        Period = 1000 / (24 * 128);
    }

    void Update(MIDILAR::SystemFoundation::Clock::TimePoint SystemTime) override {
        // Generate a clock tick every 24 ticks (MIDI clock resolution)
        
        if (SystemTime - lastTick >= Period) {
            lastTick += Period;
            ClockTick();
        }
    }

    void ClockTick() override {
        uint8_t clockMessage[] = {0xF8}; // MIDI Clock message
        MidiOutput(clockMessage, sizeof(clockMessage));
    }
};

void MidiOutCallback(const uint8_t* Message, size_t size){

    std::cout << "Generated MIDI clock: ";
    for (size_t i = 0; i < size; ++i) {
        std::cout << std::hex << static_cast<int>(Message[i]) << " ";
    }
    std::cout << std::endl;
}

// Example usage
int main() {
    MidiClockProcessor clockProcessor;

    // Setting up the output callback
    clockProcessor.BindMidiOut(MidiOutCallback);

    // Simulating system time updates
    for (int i = 0; i < 100; ++i) {
        clockProcessor.Update(i); // Simulating system ticks
    }

    return 0;
}
