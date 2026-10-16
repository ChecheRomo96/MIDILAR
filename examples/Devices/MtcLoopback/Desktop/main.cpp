#include "../Shared.h"

#include <chrono>
#include <cstdio>

namespace {

uint32_t Micros() {
    static const auto start = std::chrono::steady_clock::now();
    return static_cast<uint32_t>(
        std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count());
}

} // namespace

int main() {
    using MIDILAR::Protocol::TimeCode;
    using MIDILAR::Protocol::TimeCodeRate;

    MIDILARExamples::Devices::MtcLoopback::MtcLoopback loopback(Micros);
    loopback.Start(TimeCode::From(1, 0, 0, 0, TimeCodeRate::Fps25));
    std::printf("========================================\n");
    std::printf(" MIDILAR :: Devices / MtcLoopback\n");
    std::printf(" received time, once per second (25 fps)\n");
    std::printf("========================================\n");
    uint32_t next = Micros() + 1000000;
    for (int line = 0; line < 3;) {
        loopback.Update();
        if (static_cast<int32_t>(Micros() - next) >= 0) {
            next += 1000000;
            ++line;
            const TimeCode t = loopback.Received();
            std::printf(" %02u:%02u:%02u:%02u  (%u bytes)\n", t.Hours(), t.Minutes(), t.Seconds(), t.Frames(),
                        static_cast<unsigned>(loopback.Bytes()));
        }
    }
    std::printf("========================================\n");
    return 0;
}
