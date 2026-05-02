#include <MIDILAR_MidiCore.h>

using namespace MIDILAR::MidiCore;

namespace{
class MidiPrinter {
public:
    MidiPrinter(size_t BufferSize = 3) : _parser(BufferSize) {
        _parser.BindChannelVoiceCallback(this, &MidiPrinter::StaticChannelVoice);
        _parser.BindControlChangeCallback(this, &MidiPrinter::StaticControlChange);
        _parser.BindRealTimeCallback(this, &MidiPrinter::StaticRealTime);
        _parser.BindSysExCallback(this, &MidiPrinter::StaticSysEx);
        _parser.BindSystemCommonCallback(this, &MidiPrinter::StaticSystemCommon);
        _parser.BindMTCCallback(this, &MidiPrinter::StaticMTC);
        _parser.BindMSCCallback(this, &MidiPrinter::StaticMSC);
        _parser.BindDefaultCallback(this, &MidiPrinter::StaticDefault);
    }

    void Feed(const uint8_t* data, size_t size) {
        _parser.ProcessData(data, size);
    }

private:
    MessageParser _parser;

    // Static -> Instance dispatcher
    static void StaticChannelVoice(void* ctx, const uint8_t* data, size_t size) { static_cast<MidiPrinter*>(ctx)->HandleChannelVoice(data, size); }
    static void StaticControlChange(void* ctx, const uint8_t* data, size_t size) { static_cast<MidiPrinter*>(ctx)->HandleControlChange(data, size); }
    static void StaticRealTime(void* ctx, const uint8_t* data, size_t size) { static_cast<MidiPrinter*>(ctx)->HandleRealTime(data, size); }
    static void StaticSysEx(void* ctx, const uint8_t* data, size_t size) { static_cast<MidiPrinter*>(ctx)->HandleSysEx(data, size); }
    static void StaticSystemCommon(void* ctx, const uint8_t* data, size_t size) { static_cast<MidiPrinter*>(ctx)->HandleSystemCommon(data, size); }
    static void StaticMTC(void* ctx, const uint8_t* data, size_t size) { static_cast<MidiPrinter*>(ctx)->HandleMTC(data, size); }
    static void StaticMSC(void* ctx, const uint8_t* data, size_t size) { static_cast<MidiPrinter*>(ctx)->HandleMSC(data, size); }
    static void StaticDefault(void* ctx, const uint8_t* data, size_t size) { static_cast<MidiPrinter*>(ctx)->HandleDefault(data, size); }

    // Instance handlers
    void HandleChannelVoice(const uint8_t* data, size_t size) { PrintHex("[Channel Voice]", data, size); }
    void HandleControlChange(const uint8_t* data, size_t size) { PrintHex("[Control Change]", data, size); }
    void HandleRealTime(const uint8_t* data, size_t size) { PrintHex("[Real Time]", data, size); }
    void HandleSysEx(const uint8_t* data, size_t size) { PrintHex("[SysEx]", data, size); }
    void HandleSystemCommon(const uint8_t* data, size_t size) { PrintHex("[System Common]", data, size); }
    void HandleMTC(const uint8_t* data, size_t size) { PrintHex("[MIDI Time Code]", data, size); }
    void HandleMSC(const uint8_t* data, size_t size) { PrintHex("[MIDI Show Control]", data, size); }
    void HandleDefault(const uint8_t* data, size_t size) { PrintHex("[Default]", data, size); }

    void PrintHex(const char* label, const uint8_t* data, size_t size) {
        Serial.print(label);
        Serial.print(" ");
        for (size_t i = 0; i < size; ++i) {
            if (data[i] < 0x10) Serial.print("0");
            Serial.print(data[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }
};
}

MidiPrinter printer(32);

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("Running Instance Callback Test...");

    // Feed example messages
    uint8_t noteOn[] = { 0x90, 0x3C, 100 };
    uint8_t cc[]     = { 0xB0, 1, 64 };
    uint8_t clock[]  = { 0xF8 };
    uint8_t sysex[]  = { 0xF0, 0x7D, 0x01, 0x02, 0xF7 };

    printer.Feed(noteOn, sizeof(noteOn));
    printer.Feed(cc, sizeof(cc));
    printer.Feed(clock, sizeof(clock));
    printer.Feed(sysex, sizeof(sysex));

    Serial.println("Instance Callback Test complete.");
}

void loop() {
  // No loop processing needed
}
