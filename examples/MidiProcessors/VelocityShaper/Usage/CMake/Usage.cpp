#include <iostream>
#include <vector>
#include <string>
#include <MIDILAR_MidiProcessors.h>  // Include VelocityShaper
#include <MIDILAR_MidiFoundation.h>

using namespace MIDILAR::MidiProcessors;

// ===============================
// Velocity Shaper Processor
// ===============================
VelocityShaper velocityShaper;

// ===============================
// Sample MIDI Data (Simulated Input)
// ===============================
std::vector<std::vector<uint8_t>> midiMessages = {
    {0x90, 0x3C, 0x64},  // Note On, Middle C, Velocity 100
    {0x90, 0x40, 0x50},  // Note On, E4, Velocity 80
    {0xB0, 0x07, 0x7F}   // Control Change, Volume Max (should remain unchanged)
};

// ===============================
// MIDI Output Callback
// ===============================
// This function prints the modified MIDI messages.
void MidiOutCallback(const uint8_t* Message, size_t size) {
    std::cout << "Processed MIDI: ";
    for (size_t i = 0; i < size; ++i) {
        std::cout << std::hex << static_cast<int>(Message[i]) << " ";
    }
    std::cout << std::endl;
}

// ===============================
// Function to Process MIDI Messages
// ===============================
void ProcessMidiBatch() {
    std::cout << "\nProcessing MIDI messages..." << std::endl;

    for (auto& message : midiMessages) {
        // Display Original MIDI Message
        std::cout << "Original MIDI: ";
        for (uint8_t byte : message) std::cout << std::hex << static_cast<int>(byte) << " ";
        std::cout << std::endl;

        // Process MIDI message
        velocityShaper.MidiInput(message.data(), message.size());
    }
}

// ===============================
// Main Function
// ===============================
int main() {
    std::cout << "MIDI Velocity Shaper Processor Initialized..." << std::endl;
    std::cout << "Use 'mX.X' to set morph, 'eX.X' to set exp gain, 'p' to process MIDI, 'q' to quit.\n" << std::endl;

    // Bind MIDI output callback to the processor
    velocityShaper.BindMidiOut(MidiOutCallback);

    // Configure initial shaping parameters
    velocityShaper.SetInputChannels(0b1);
    velocityShaper.SetMorph(0.5f);
    velocityShaper.SetExpGain(2.0f);

    // Interactive Parameter Tuning
    std::string command;
    while (true) {
        std::cout << "\nEnter command: ";
        std::cin >> command;

        if (command == "q") break; // Quit
        if (command == "p") {
            ProcessMidiBatch();  // Process MIDI messages
            continue;
        }

        if (command[0] == 'm') {
            float morph = std::stof(command.substr(1));
            velocityShaper.SetMorph(morph);
            std::cout << "Morph set to: " << morph << std::endl;
        }
        else if (command[0] == 'e') {
            float expGain = std::stof(command.substr(1));
            velocityShaper.SetExpGain(expGain);
            std::cout << "Exp Gain set to: " << expGain << std::endl;
        }
    }

    return 0;
}
