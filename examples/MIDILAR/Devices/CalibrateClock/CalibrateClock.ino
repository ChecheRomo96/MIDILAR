// Measures this board's oscillator error against a reference MIDI clock and
// stores the correction in EEPROM.
//
// 1. Play MIDI clock at exactly 120 BPM into the board (a DAW through a MIDI
//    shield, or through Hairless MIDI with -DMIDILAR_EXAMPLE_MIDI_BAUD=115200).
// 2. After 30 seconds of clock the LED lights and the correction is stored.
// 3. Open the serial monitor (115200 baud with a bridge): on every start the
//    sketch prints the stored correction, for example "stored ppm: -500".
//    Use it as -DMIDILAR_EXAMPLE_CLOCK_PPM=<ppm> in ClockOut and MtcOut.

#include <EEPROM.h>
#include <MIDILAR_Devices.h>

#include "Shared.h"

#ifndef MIDILAR_EXAMPLE_MIDI_BAUD
    #define MIDILAR_EXAMPLE_MIDI_BAUD 31250
#endif

namespace {

constexpr int EepromAddress = 0;
constexpr uint32_t Magic = 0x4D43414Cu; // "MCAL"

struct Stored {
    uint32_t magic;
    int32_t ppm;
};

uint32_t Micros() {
    return micros();
}

MIDILARExamples::Devices::CalibrateClock::CalibrateClock device(Micros);
bool saved = false;

} // namespace

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(MIDILAR_EXAMPLE_MIDI_BAUD);
    Stored stored{};
    EEPROM.get(EepromAddress, stored);
    if (stored.magic == Magic) {
        Serial.print("stored ppm: ");
        Serial.println(stored.ppm);
    } else {
        Serial.println("stored ppm: none");
    }
    device.Start(12000, 24u * 60u);
}

void loop() {
    while (Serial.available() > 0) {
        device.Receive(static_cast<uint8_t>(Serial.read()));
    }
    if (device.IsDone() && !saved) {
        saved = true;
        const Stored stored{Magic, device.Ppm()};
        EEPROM.put(EepromAddress, stored);
        digitalWrite(LED_BUILTIN, HIGH);
    }
}
