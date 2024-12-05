#include <MidiProcessor/MidiProcessor.h>
#include <iostream>
#include <vector>

class MidiEchoProcessor : public MIDILAR::MidiProcessor {
public:
    MidiEchoProcessor() {
        SetCapabilities(static_cast<uint32_t>(Capabilities::MidiIn) |
                        static_cast<uint32_t>(Capabilities::MidiOut));
    }

    void MidiInput(uint8_t* Message, size_t size) override {
        std::cout << "Received MIDI message: ";
        for (size_t i = 0; i < size; ++i) {
            std::cout << std::hex << static_cast<int>(Message[i]) << " ";
        }
        std::cout << std::endl;

        // Echo the message back out
        MidiOutput(Message, size);
    }
};

// Example usage
int main() {
    MidiEchoProcessor echoProcessor;

    // Setting up the output callback
    echoProcessor.MidiOutApiLink([](uint8_t* Message, size_t size) {
        std::cout << "Echoed MIDI message: ";
        for (size_t i = 0; i < size; ++i) {
            std::cout << std::hex << static_cast<int>(Message[i]) << " ";
        }
        std::cout << std::endl;
    });

    // Simulating incoming MIDI data
    uint8_t midiMessage[] = {0x90, 0x45, 0x60}; // Note On message
    echoProcessor.MidiInput(midiMessage, sizeof(midiMessage));

    return 0;
}