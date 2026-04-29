#include <MIDILAR/dspFoundation/Generators/Shaping/AtanNorm.h>

constexpr size_t BufferSize = 129;
float buffer[BufferSize];

void setup() {
    Serial.begin(115200);

    while (!Serial) {
        ;
    }

    const float Drive = 4.0f;
    const float Bias = 0.25f;

    MIDILAR::dspFoundation::Generators::Shaping::AtanNorm<float>::Generate(
        buffer,
        BufferSize,
        Drive, Bias
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
