#include <MIDILAR/dspFoundation/Generators/Shaping/TanhHalfNorm.h>

constexpr size_t BufferSize = 64;
float buffer[BufferSize];

void setup() {
    Serial.begin(115200);

    while (!Serial) {
        ;
    }

    const float Drive = 3.0f;

    MIDILAR::dspFoundation::Generators::Shaping::TanhHalfNorm<float>::Generate(
        buffer,
        BufferSize,
        Drive
    );

    Serial.println("index,value");

    for (size_t i = 0; i < BufferSize; ++i) {
        Serial.print(i);
        Serial.print(',');
        Serial.println(buffer[i], 6);
    }
}

void loop() {
}
