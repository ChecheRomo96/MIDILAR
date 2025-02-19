#include <Arduino.h>
#include <MIDILAR_MidiFoundation.h>

// ===============================
// SERIAL & MIDI CONFIGURATION
// ===============================
#define MIDI_BAUD_RATE 31250  // Standard MIDI baud rate for Serial1
#define MIDI_CLOCK_MESSAGE 0xF8  // MIDI Clock status byte
#define MIDI_PPQN 24  // MIDI clock resolution (24 pulses per quarter note)
#define TARGET_BPM 128.0  // Desired BPM

// ===============================
// MIDI Clock Processor Class (Base: Processor)
// ===============================
class MidiClockProcessor : public MIDILAR::MidiFoundation::Processor {
private:
    uint32_t periodInt;  // Integer part of the period in microseconds
    uint32_t lastTick;   // Last tick timestamp in microseconds
    float errorCorrection;  // Fractional part error accumulator

public:
    MidiClockProcessor() {
        SetCapabilities(static_cast<uint32_t>(Capabilities::MidiOut) |
                        static_cast<uint32_t>(Capabilities::InternalClock));

        lastTick = micros();
        errorCorrection = 0.0f;

        // **Compute exact period in microseconds**
        float exactPeriod = (60000000.0f / (MIDI_PPQN * TARGET_BPM));

        // **Extract integer and fractional parts**
        periodInt = static_cast<uint32_t>(exactPeriod);  // Integer part
        errorCorrection = exactPeriod - periodInt;  // Store fractional error
    }

    void Update(MIDILAR::SystemFoundation::Clock::TimePoint SystemTime) override {
        uint32_t now = micros();
        
        // **Check if enough time has passed for the next MIDI clock tick**
        if (now - lastTick >= periodInt) {
            lastTick += periodInt;  // Move forward by period

            // **Accumulate floating-point rounding error**
            static float accumulatedError = 0.0f;
            accumulatedError += errorCorrection;

            // **Apply error correction if it accumulates past 1 µs**
            if (accumulatedError >= 1.0f) {
                lastTick += 1;  // Adjust by 1 microsecond
                accumulatedError -= 1.0f;
            }

          uint8_t clockMessage = MIDI_CLOCK_MESSAGE;
          MidiOutput(&clockMessage, 1);  // Send MIDI Clock message over Serial1
        }
    }

};

// ===============================
// MIDI Output Callback
// ===============================
void MidiOutCallback(const uint8_t* Message, size_t size) {
    Serial1.write(Message, size);
}

// ===============================
// Arduino Setup & Loop
// ===============================
MidiClockProcessor clockProcessor;  // Instantiate the MIDI clock processor

void setup() {
    Serial.begin(115200);  // Debug output to Serial Monitor
    Serial1.begin(MIDI_BAUD_RATE);  // MIDI OUT on Serial1

    // Bind MIDI output callback
    clockProcessor.BindMidiOut(MidiOutCallback);

    Serial.println("MIDI Clock Processor Initialized...");
}

void loop() {
    clockProcessor.Update(micros());  // Update clock processor with system time
}