#include "../Shared.h"

#include <chrono>
#include <cstdio>

namespace {

uint32_t Micros() {
    static const auto start = std::chrono::steady_clock::now();
    return static_cast<uint32_t>(
        std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count());
}

unsigned g_clocks = 0;

void Write(uint8_t byte) {
    g_clocks += byte == 0xF8 ? 1u : 0u;
}

} // namespace

int main() {
    MIDILARExamples::Devices::ClockOut::ClockOut out(Micros);
    out.Start(12000);
    // Play exactly one second of MIDI clock at 120 BPM.
    const uint32_t end = Micros() + 1000000;
    while (static_cast<int32_t>(Micros() - end) < 0) {
        out.Update(Write);
    }
    std::printf("========================================\n");
    std::printf(" MIDILAR :: Devices / ClockOut\n");
    std::printf("========================================\n");
    std::printf(" 120 BPM for one second\n");
    std::printf(" timing clocks sent .. %u (expected 48-49)\n", g_clocks);
    std::printf(" position ............ %u clocks\n", static_cast<unsigned>(out.Position()));
    std::printf("========================================\n");
    return 0;
}
