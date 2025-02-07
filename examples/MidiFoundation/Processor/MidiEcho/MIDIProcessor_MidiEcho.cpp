#include <SystemFoundation/Clock/Clock.h>
#include <MidiFoundation/Processor/Processor.h>

#include <iostream>

class MidiEchoProcessor : public MIDILAR::MidiFoundation::Processor {
public:
    MidiEchoProcessor() {
        SetCapabilities(static_cast<uint32_t>(Capabilities::MidiIn) |
                        static_cast<uint32_t>(Capabilities::MidiOut));
    }

    void MidiInput(const uint8_t* Message, size_t size) override {
        std::cout << "Received MIDI message: ";
        for (size_t i = 0; i < size; ++i) {
            std::cout << std::hex << static_cast<int>(Message[i]) << " ";
        }
        std::cout << std::endl;

        // Echo the message back out
        MidiOutput(Message, size);
    }
};

void MidiOutCallback(const uint8_t* Message, size_t size){

    std::cout << "Echoed MIDI message: ";
    for (size_t i = 0; i < size; ++i) {
        std::cout << std::hex << static_cast<int>(Message[i]) << " ";
    }
    std::cout << std::endl;
}

// Example usage
int main() {
    MidiEchoProcessor echoProcessor;

    // Setting up the output callback
    echoProcessor.BindMidiOut(MidiOutCallback);

    // Simulating incoming MIDI data
    uint8_t midiMessage[] = {0x90, 0x45, 0x60}; // Note On message
    echoProcessor.MidiInput(midiMessage, sizeof(midiMessage));

    return 0;
}