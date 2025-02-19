#include <iostream>
#include <queue>
#include <vector>
#include <iomanip>  // For formatting hex output
#include <MIDILAR_MidiProcessors.h>  // Include ChannelReassign
#include <MIDILAR_MidiFoundation.h>

using namespace MIDILAR::MidiProcessors;

// ===============================
// MIDI SIMULATION CONFIGURATION
// ===============================
#define INPUT_CHANNEL  0b101010  // Incoming MIDI Channel
#define OUTPUT_CHANNEL 0b1  // Reassigned MIDI Channel

// ===============================
// Mock MIDI Input Queue
// ===============================
std::queue<std::vector<uint8_t>> midiInputQueue;

// ===============================
// MIDI Output Callback (Mock Serial Output)
// ===============================
// This function prints the reassigned MIDI messages.
void MidiOutCallback(const uint8_t* Message, size_t size) {
    std::cout << "Reassigned MIDI Message: ";
    for (size_t i = 0; i < size; ++i) {
        std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(Message[i]) << " ";
    }
    std::cout << std::endl;
}

// ===============================
// Simulated MIDI Input Function
// ===============================
// This function pushes simulated MIDI messages into the queue.
void MockMidiInput() {
    // Example: Note On (0x90), Note 60 (0x3C), Velocity 100 (0x64), Channel 1
    std::vector<uint8_t> midiMessage = {0x91 , 0x3C, 0x64};
    midiInputQueue.push(midiMessage);

    // Example: Control Change (0xB0), Controller 7 (0x07), Value 127 (0x7F), Channel 1
    midiMessage = {0xB1, 0x07, 0x7F};
    midiInputQueue.push(midiMessage);
    
    // Example: Note On (0x90), Note 60 (0x3C), Velocity 100 (0x64), Channel 1
    midiMessage = {0x93 , 0x3C, 0x64};
    midiInputQueue.push(midiMessage);

    // Example: Control Change (0xB0), Controller 7 (0x07), Value 127 (0x7F), Channel 1
    midiMessage = {0xB3, 0x07, 0x7F};
    midiInputQueue.push(midiMessage);
    
    // Example: Note On (0x90), Note 60 (0x3C), Velocity 100 (0x64), Channel 1
    midiMessage = {0x95 , 0x3C, 0x64};
    midiInputQueue.push(midiMessage);

    // Example: Control Change (0xB0), Controller 7 (0x07), Value 127 (0x7F), Channel 1
    midiMessage = {0xB5, 0x07, 0x7F};
    midiInputQueue.push(midiMessage);
}

// ===============================
// Channel Reassign Processor
// ===============================
ChannelReassign channelReassignProcessor;

// ===============================
// Main Function
// ===============================
int main() {
    std::cout << "MIDI Channel Reassign Processor Initialized...\n" << std::endl;

    // Bind MIDI output callback to the processor
    channelReassignProcessor.BindMidiOut(MidiOutCallback);
    channelReassignProcessor.SetInputChannels(INPUT_CHANNEL);
    channelReassignProcessor.SetOutputChannels(OUTPUT_CHANNEL);

    // Simulate MIDI input
    MockMidiInput();

    // Process all incoming MIDI messages
    while (!midiInputQueue.empty()) {
        std::vector<uint8_t> midiMessage = midiInputQueue.front();
        midiInputQueue.pop();

        // **Print incoming message before processing**
        std::cout << "Incoming MIDI Message: ";
        for (size_t i = 0; i < midiMessage.size(); ++i) {
            std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') 
                      << static_cast<int>(midiMessage[i]) << " ";
        }
        std::cout << std::endl;

        // Process the input MIDI message
        channelReassignProcessor.MidiInput(midiMessage.data(), midiMessage.size());
    }

    return 0;
}
