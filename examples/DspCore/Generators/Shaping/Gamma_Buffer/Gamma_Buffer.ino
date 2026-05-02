#include <MIDILAR/DspCore/Generators/Shaping/Gamma.h>

constexpr size_t BufferSize = 64;
float buffer[BufferSize];

void setup() {
    Serial.begin(115200);

    while (!Serial) {
        ;
    }

    const float GammaValue = 2.2f;

    MIDILAR::DspCore::Generators::Shaping::Gamma<float>::Generate(
        buffer,
        BufferSize,
        GammaValue
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
