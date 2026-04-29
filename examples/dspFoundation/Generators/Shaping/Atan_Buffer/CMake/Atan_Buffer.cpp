#include <MIDILAR/dspFoundation/Generators/Shaping/Atan.h>
#include <stdio.h>
#include <stddef.h>

int main() {
    constexpr size_t BufferSize = 129;
    float buffer[BufferSize];

    const float Drive = 4.0f;
    const float Bias = 0.25f;

    MIDILAR::dspFoundation::Generators::Shaping::Atan<float>::Generate(
        buffer,
        BufferSize,
        Drive, Bias
    );

    printf("index,value\n");

    for (size_t i = 0; i < BufferSize; ++i) {
        printf("%zu,%f\n", i, buffer[i]);
    }

    return 0;
}
