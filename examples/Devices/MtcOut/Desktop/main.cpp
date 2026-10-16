#include "../Shared.h"

#include <chrono>
#include <cstdio>

namespace {

uint32_t Micros() {
    static const auto start = std::chrono::steady_clock::now();
    return static_cast<uint32_t>(
        std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count());
}

unsigned g_quarterFrames = 0;
unsigned g_fullFrameBytes = 0;
bool g_inSysEx = false;

void Write(uint8_t byte) {
    if (byte == 0xF0) {
        g_inSysEx = true;
    }
    if (g_inSysEx) {
        ++g_fullFrameBytes;
    }
    if (byte == 0xF7) {
        g_inSysEx = false;
    }
    g_quarterFrames += byte == 0xF1 ? 1u : 0u;
}

void Print(const char* label, const MIDILAR::Protocol::TimeCode& time) {
    std::printf(" %s %02u:%02u:%02u:%02u\n", label, time.Hours(), time.Minutes(), time.Seconds(), time.Frames());
}

} // namespace

int main() {
    using MIDILAR::Protocol::TimeCode;
    using MIDILAR::Protocol::TimeCodeRate;

    MIDILARExamples::Devices::MtcOut::MtcOut out(Micros);
    const TimeCode start = TimeCode::From(1, 0, 0, 0, TimeCodeRate::Fps25);
    out.Start(start, Write);
    // Play exactly one second of MTC at 25 fps.
    const uint32_t end = Micros() + 1000000;
    while (static_cast<int32_t>(Micros() - end) < 0) {
        out.Update(Write);
    }
    std::printf("========================================\n");
    std::printf(" MIDILAR :: Devices / MtcOut\n");
    std::printf("========================================\n");
    Print("start ............", start);
    Print("after one second .", out.Time());
    std::printf(" full frame bytes  %u (expected 10)\n", g_fullFrameBytes);
    std::printf(" quarter frames .. %u (expected 100-101)\n", g_quarterFrames);
    std::printf("========================================\n");
    return 0;
}
