#include <MidiCore/MessageParser/MessageParser.h>
#include <iostream>
#include <cstdint>

using namespace MIDILAR::MidiCore;

namespace{
class MidiPrinter {
public:
    MidiPrinter(size_t BufferSize = 3) : _parser(BufferSize) {
        _parser.BindChannelVoiceCallback<MidiPrinter, &MidiPrinter::HandleChannelVoice>(this);
        _parser.BindControlChangeCallback<MidiPrinter, &MidiPrinter::HandleControlChange>(this);
        _parser.BindRealTimeCallback<MidiPrinter, &MidiPrinter::HandleRealTime>(this);
        _parser.BindSysExCallback<MidiPrinter, &MidiPrinter::HandleSysEx>(this);
        _parser.BindSystemCommonCallback<MidiPrinter, &MidiPrinter::HandleSystemCommon>(this);
        _parser.BindMTCCallback<MidiPrinter, &MidiPrinter::HandleMTC>(this);
        _parser.BindMSCCallback<MidiPrinter, &MidiPrinter::HandleMSC>(this);
        _parser.BindDefaultCallback<MidiPrinter, &MidiPrinter::HandleDefault>(this);
        
    }

    void Feed(const uint8_t* data, size_t size) {
        _parser.ProcessData(data, size);
    }

private:
    MessageParser _parser;

    // Instance handlers
    void HandleChannelVoice(const uint8_t* data, size_t size) {
        std::cout << "[Channel Voice] ";
        PrintHex(data, size);
    }

    void HandleControlChange(const uint8_t* data, size_t size) {
        std::cout << "[Control Change] ";
        PrintHex(data, size);
    }

    void HandleRealTime(const uint8_t* data, size_t size) {
        std::cout << "[Real Time] ";
        PrintHex(data, size);
    }

    void HandleSysEx(const uint8_t* data, size_t size) {
        std::cout << "[SysEx] ";
        PrintHex(data, size);
    }

    void HandleSystemCommon(const uint8_t* data, size_t size) {
        std::cout << "[System Common] ";
        PrintHex(data, size);
    }

    void HandleMTC(const uint8_t* data, size_t size) {
        std::cout << "[MIDI Time Code] ";
        PrintHex(data, size);
    }

    void HandleMSC(const uint8_t* data, size_t size) {
        std::cout << "[MIDI Show Control] ";
        PrintHex(data, size);
    }

    void HandleDefault(const uint8_t* data, size_t size) {
        std::cout << "[Default] ";
        PrintHex(data, size);
    }

    void PrintHex(const uint8_t* data, size_t size) {
        for (size_t i = 0; i < size; ++i) {
            std::cout << "0x" << std::hex << (int)data[i] << " ";
        }
        std::cout << std::dec << std::endl;
    }
};
}

int main() {
    MidiPrinter printer;

    // Example: Note On (0x90), Note 60, Velocity 100
    uint8_t noteOn[] = { 0x90, 0x3C, 100 };
    printer.Feed(noteOn, sizeof(noteOn));

    // Example: Control Change (0xB0), Mod Wheel (1), Value 64
    uint8_t cc[] = { 0xB0, 1, 64 };
    printer.Feed(cc, sizeof(cc));

    // Example: Real-Time Clock
    uint8_t clock[] = { 0xF8 };
    printer.Feed(clock, sizeof(clock));

    // Example: SysEx (0xF0 ... 0xF7)
    uint8_t sysex[] = { 0xF0, 0x7D, 0x01, 0x02, 0xF7 };
    printer.Feed(sysex, sizeof(sysex));

    return 0;
}
