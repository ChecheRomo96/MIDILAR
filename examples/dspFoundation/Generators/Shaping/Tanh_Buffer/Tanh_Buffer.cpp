#include <MIDILAR/dspFoundation/Generators/Shaping/Tanh.h>
#include <stdio.h>
#include <stddef.h>

int main() {
    constexpr size_t BufferSize = 129;
    float buffer[BufferSize];

    const float Drive = 3.0f;

    MIDILAR::dspFoundation::Generators::Shaping::Tanh<float>::Generate(
        buffer,
        BufferSize,
        Drive
    );

    printf("index,value\n");

    for (size_t i = 0; i < BufferSize; ++i) {
        printf("%zu,%f\n", i, buffer[i]);
    }

    return 0;
}
