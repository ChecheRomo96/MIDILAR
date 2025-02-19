#include <MIDILAR_MidiFoundation.h>
#include <stdint.h>
#include <iostream>

int main(int argc, char *argv[]) {
    using namespace MIDILAR::MidiFoundation;

    // Create a Note On message
    Message noteOnMessage;
    noteOnMessage.NoteOn(60, 120, 1);
    std::cout << "Note On Message: ";
    for (size_t i = 0; i < noteOnMessage.size(); i++) {
        std::cout << std::to_string(noteOnMessage.Buffer()[i]);
        if (i < noteOnMessage.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    // Create a Note Off message
    Message noteOffMessage;
    noteOffMessage.NoteOff(60, 0, 1);
    std::cout << "Note Off Message: ";
    for (size_t i = 0; i < noteOffMessage.size(); i++) {
        std::cout << std::to_string(noteOffMessage.Buffer()[i]);
        if (i < noteOffMessage.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    // Create a Control Change message (Modulation Wheel on Channel 1)
    Message controlChangeMessage;
    controlChangeMessage.ControlChange(1, 64, 1);
    std::cout << "Control Change (Modulation Wheel) Message: ";
    for (size_t i = 0; i < controlChangeMessage.size(); i++) {
        std::cout << std::to_string(controlChangeMessage.Buffer()[i]);
        if (i < controlChangeMessage.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    // Create a Program Change message (Change to instrument #10 on Channel 1)
    Message programChangeMessage;
    programChangeMessage.ProgramChange(10, 1);
    std::cout << "Program Change Message: ";
    for (size_t i = 0; i < programChangeMessage.size(); i++) {
        std::cout << std::to_string(programChangeMessage.Buffer()[i]);
        if (i < programChangeMessage.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    // Create a Pitch Bend message (Centered Pitch Bend on Channel 1)
    Message pitchBendMessage;
    uint16_t bend = 8192;
    pitchBendMessage.PitchBend(bend, 1); // 8192 = Center
    std::cout << "Pitch Bend Message: ";
    for (size_t i = 0; i < pitchBendMessage.size(); i++) {
        std::cout << std::to_string(pitchBendMessage.Buffer()[i]);
        if (i < pitchBendMessage.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    // Create a System Exclusive (SysEx) message
    uint8_t sysExData[] = { 0x7E, 0x7F, 0x09, 0x01 }; // Example SysEx data
    Message sysExMessage;
    sysExMessage.SystemExclusive(sysExData, sizeof(sysExData));
    std::cout << "SysEx Message: ";
    for (size_t i = 0; i < sysExMessage.size(); i++) {
        std::cout << std::to_string(sysExMessage.Buffer()[i]);
        if (i < sysExMessage.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    return 0;
}
