#include "../Shared.h"

#include <cstdio>

int main() {
    MIDILARExamples::Devices::TransposeThru::Thru thru;
    thru.SetSemitones(2);

    // C4 on, C#4 on with running status, a clock, C4 off.
    const uint8_t input[] = {0x90, 60, 100, 61, 100, 0xF8, 0x80, 60, 0};

    std::printf("========================================\n");
    std::printf(" MIDILAR :: Devices / TransposeThru\n");
    std::printf(" +2 semitones, kept on C major\n");
    std::printf("========================================\n");
    std::printf(" in :");
    for (uint8_t byte : input) {
        std::printf(" %02X", byte);
    }
    std::printf("\n out:");
    for (uint8_t byte : input) {
        uint8_t out[MIDILARExamples::Devices::TransposeThru::Thru::Capacity];
        const uint8_t count = thru.Process(byte, out);
        for (uint8_t i = 0; i < count; ++i) {
            std::printf(" %02X", out[i]);
        }
    }
    std::printf("\n========================================\n");
    return 0;
}
