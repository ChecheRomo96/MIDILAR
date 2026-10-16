#include "../Shared.h"

#include <cstdio>

#include <MIDILAR/Protocol/Midi1Encoder.h>

namespace {

// Simulated time: the reference runs on true time, this "device" on a clock
// that is 500 ppm slow.
uint64_t g_trueMicros = 0;
uint32_t TrueMicros() { return static_cast<uint32_t>(g_trueMicros); }
uint32_t SlowMicros() { return static_cast<uint32_t>(g_trueMicros * 9995 / 10000); }

MIDILARExamples::Devices::CalibrateClock::CalibrateClock* g_device = nullptr;
MIDILAR::Protocol::Midi1Encoder g_encoder;

void Wire(const MIDILAR::Protocol::Packet& packet) {
    uint8_t bytes[MIDILAR::Protocol::Midi1Encoder::MaxBytes];
    const uint8_t count = g_encoder.Encode(packet, bytes);
    for (uint8_t i = 0; i < count; ++i) {
        g_device->Receive(bytes[i]);
    }
}

} // namespace

int main() {
    Foundation::Time::Clock referenceClock(TrueMicros, Foundation::Time::Frequency(1000000, 1));
    MIDILAR::Devices::ClockGenerator reference(referenceClock);
    MIDILARExamples::Devices::CalibrateClock::CalibrateClock device(SlowMicros);
    g_device = &device;
    reference.Output().Bind(Wire);

    device.Start(12000, 24 * 60);
    reference.Update();
    while (!device.IsDone()) {
        g_trueMicros += 100;
        reference.Update();
    }
    std::printf("========================================\n");
    std::printf(" MIDILAR :: Devices / CalibrateClock\n");
    std::printf(" simulated device oscillator 500 ppm slow\n");
    std::printf("========================================\n");
    std::printf(" measured correction .. %d ppm (expected -500)\n", static_cast<int>(device.Ppm()));
    std::printf("========================================\n");
    return 0;
}
