#include <MIDILAR/dspFoundation/Generators/Shaping/AtanHalf.h>
#include <stdio.h>
#include <stddef.h>

int main() {
    constexpr size_t BufferSize = 64;
    float buffer[BufferSize];

    const float Drive = 4.0f;
    const float Bias = 0.0f;

    MIDILAR::dspFoundation::Generators::Shaping::AtanHalf<float>::Generate(
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
