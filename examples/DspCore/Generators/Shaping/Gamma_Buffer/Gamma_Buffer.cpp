#include <DspCore/Generators/Shaping/Gamma.h>
#include <stdio.h>
#include <stddef.h>

int main() {
    constexpr size_t BufferSize = 64;
    float buffer[BufferSize];

    const float GammaValue = 2.2f;

    MIDILAR::DspCore::Generators::Shaping::Gamma<float>(
        buffer,
        BufferSize,
        GammaValue
    );

    printf("index,value\n");

    for (size_t i = 0; i < BufferSize; ++i) {
        printf("%zu,%f\n", i, buffer[i]);
    }

    return 0;
}
