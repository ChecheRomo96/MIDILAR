#include <MIDILAR.h>

using namespace MIDILAR::dspFoundation::LUT;

LUT1D<float> waveform(4);

float phase = 0.0f;
float phaseIncrement = 0.125f;

void setup() {
    Serial.begin(115200);

    waveform.SetBinValue(0,  0.0f);
    waveform.SetBinValue(1,  1.0f);
    waveform.SetBinValue(2,  0.0f);
    waveform.SetBinValue(3, -1.0f);

    Serial.println("ManualOscillator example");
}

void loop() {
    size_t index = static_cast<size_t>(phase * static_cast<float>(waveform.Size()));

    if (index >= waveform.Size()) {
        index = waveform.Size() - 1;
    }

    float sample = waveform.GetValue(index);

    Serial.print("phase = ");
    Serial.print(phase);
    Serial.print(", index = ");
    Serial.print(index);
    Serial.print(", sample = ");
    Serial.println(sample);

    phase += phaseIncrement;

    if (phase >= 1.0f) {
        phase -= 1.0f;
    }

    delay(250);
}
