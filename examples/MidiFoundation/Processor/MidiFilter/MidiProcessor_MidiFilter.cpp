#include <SystemFoundation/Clock/Clock.h>
#include <MidiFoundation/Processor/Processor.h>

#include <iostream>

class MidiFilterProcessor : public MIDILAR::MidiFoundation::Processor {
public:
    MidiFilterProcessor() {
        SetCapabilities(static_cast<uint32_t>(Capabilities::MidiIn) |
                        static_cast<uint32_t>(Capabilities::MidiOut));
    }

    void MidiInput(const uint8_t* Message, size_t size) override {
        if (Message[0] == 0x90 && Message[2] < 64) { // Note On, low velocity
            std::cout << "Filtered out MIDI message." << std::endl;
            return;
        }

        // Pass through unfiltered messages
        MidiOutput(Message, size);
    }
};

void MidiOutCallback(const uint8_t* Message, size_t size){

    std::cout << "Filtered MIDI message passed through: ";
    for (size_t i = 0; i < size; ++i) {
        std::cout << std::hex << static_cast<int>(Message[i]) << " ";
    }
    std::cout << std::endl;
}

// Example usage
int main() {
    MidiFilterProcessor filterProcessor;

    // Setting up the output callback
    filterProcessor.BindMidiOut(MidiOutCallback);

    // Simulating incoming MIDI data
    uint8_t lowVelocityMessage[] = {0x90, 0x45, 0x30}; // Note On, low velocity
    uint8_t highVelocityMessage[] = {0x90, 0x45, 0x70}; // Note On, high velocity

    filterProcessor.MidiInput(lowVelocityMessage, sizeof(lowVelocityMessage));
    filterProcessor.MidiInput(highVelocityMessage, sizeof(highVelocityMessage));

    return 0;
}