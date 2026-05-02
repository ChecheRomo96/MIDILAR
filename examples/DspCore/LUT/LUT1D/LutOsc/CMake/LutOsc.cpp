#include <cstdint>
#include <cstddef>
#include <iostream>
#include <cmath>

// ------------------------------------------------------------
// Desktop replacement for Arduino micros()
// ------------------------------------------------------------
static uint32_t SimulatedMicros = 0;

uint32_t micros() {
    return SimulatedMicros;
}

// ------------------------------------------------------------
// If MIDILAR is available, use it.
// Otherwise this file includes a tiny fallback LUT/interpolation
// so the desktop architecture can still be tested standalone.
// ------------------------------------------------------------

#ifndef USE_REAL_MIDILAR

namespace MIDILAR {
namespace DspCore {

namespace Interpolation {

template <typename XType, typename YType>
YType Linear(XType x0, XType x1, YType y0, YType y1, XType x) {
    if (x1 == x0) {
        return y0;
    }

    float t = (float)(x - x0) / (float)(x1 - x0);
    return (YType)((float)y0 + ((float)y1 - (float)y0) * t);
}

} // namespace Interpolation

namespace LUT {

template <typename T>
class LUT1D {
private:
    T* Data;
    size_t Count;

public:
    explicit LUT1D(size_t size)
        : Data(new T[size]), Count(size) {}

    ~LUT1D() {
        delete[] Data;
    }

    size_t Size() const {
        return Count;
    }

    T GetValue(size_t index) const {
        return Data[index];
    }

    void SetValue(size_t index, T value) {
        Data[index] = value;
    }
};

} // namespace LUT

namespace Generators {
namespace Periodic {

template <typename T>
void SineLUT(MIDILAR::DspCore::LUT::LUT1D<T>& lut,
             T amplitude,
             T offset,
             float phaseOffset) {
    const float TwoPi = 6.28318530717958647692f;

    for (size_t i = 0; i < lut.Size(); i++) {
        float phase = ((float)i / (float)lut.Size()) + phaseOffset;
        float y = std::sin(TwoPi * phase);

        T value = (T)((float)offset + (float)amplitude * y);
        lut.SetValue(i, value);
    }
}

} // namespace Periodic
} // namespace Generators

} // namespace DspCore
} // namespace MIDILAR

#else

#include <MIDILAR_DspCore.h>

#endif

// ------------------------------------------------------------
// Simulation config
// ------------------------------------------------------------

static constexpr size_t LutSize = 32;
static constexpr size_t BufferSize = 128;

static constexpr uint32_t ClockFreq = 1000000UL;
static constexpr float fs = 1000.0f;
static constexpr float fc = 100.0f;

static constexpr uint16_t SignalRange = 0x0FFF;

uint32_t ClockPeriod = ClockFreq / (uint32_t)fs;
uint32_t LastDacRefresh = 0;

bool ActiveBuffer = false;       // false = BufferA playing, true = BufferB playing
size_t BufferPosition = 0;

uint32_t BufferA[BufferSize];
uint32_t BufferB[BufferSize];

bool BufferReadyA = false;
bool BufferReadyB = false;

size_t FillPosition = 0;
float PhaseAccumulator = 0.0f;

MIDILAR::DspCore::LUT::LUT1D<uint32_t> OscLUT(LutSize);

uint32_t GenerateNextSample();
void FillBufferStep(uint32_t* Buffer, size_t& Position, bool& Ready);
void DAC_Callback(uint16_t value);

void DAC_Callback(uint16_t value) {
    // CSV format:
    // time_us,value
    std::cout << SimulatedMicros << "," << value << "\n";
}

uint32_t GenerateNextSample() {
    float Index = PhaseAccumulator * (float)OscLUT.Size();

    size_t X0 = (size_t)Index;
    size_t X1 = X0 + 1;

    if (X1 >= OscLUT.Size()) {
        X1 = 0;
    }

    float Frac = Index - (float)X0;

    uint32_t Y0 = OscLUT.GetValue(X0);
    uint32_t Y1 = OscLUT.GetValue(X1);

    uint32_t Value = MIDILAR::DspCore::Interpolation::Linear(
        0.0f,
        1.0f,
        Y0,
        Y1,
        Frac
    );

    PhaseAccumulator += fc / fs;

    if (PhaseAccumulator >= 1.0f) {
        PhaseAccumulator -= (uint32_t)PhaseAccumulator;
    }

    return Value;
}

void FillBufferStep(uint32_t* Buffer, size_t& Position, bool& Ready) {
    if (Ready) {
        return;
    }

    Buffer[Position] = GenerateNextSample();
    Position++;

    if (Position >= BufferSize) {
        Position = 0;
        Ready = true;
    }
}

void SetupSimulation() {
    MIDILAR::DspCore::Generators::Periodic::SineLUT<uint32_t>(
        OscLUT,
        SignalRange / 2,
        SignalRange / 2,
        0.0f
    );

    FillPosition = 0;
    while (!BufferReadyA) {
        FillBufferStep(BufferA, FillPosition, BufferReadyA);
    }

    FillPosition = 0;
    while (!BufferReadyB) {
        FillBufferStep(BufferB, FillPosition, BufferReadyB);
    }

    FillPosition = 0;
    LastDacRefresh = micros();
}

void LoopSimulation() {
    uint32_t CT = micros();

    // 1. Highest priority: DAC refresh
    if ((uint32_t)(CT - LastDacRefresh) >= ClockPeriod) {
        LastDacRefresh += ClockPeriod;

        uint32_t* PlayBuffer = ActiveBuffer ? BufferB : BufferA;

        uint16_t Value = (uint16_t)PlayBuffer[BufferPosition];
        DAC_Callback(Value);

        BufferPosition++;

        if (BufferPosition >= BufferSize) {
            BufferPosition = 0;

            if (ActiveBuffer) {
                BufferReadyB = false;
                ActiveBuffer = false;
            } else {
                BufferReadyA = false;
                ActiveBuffer = true;
            }

            FillPosition = 0;
        }
    }

    // 2. Background refill of inactive buffer
    if (ActiveBuffer) {
        FillBufferStep(BufferA, FillPosition, BufferReadyA);
    } else {
        FillBufferStep(BufferB, FillPosition, BufferReadyB);
    }
}

int main() {
    SetupSimulation();

    std::cout << "time_us,value\n";

    // Run 100 ms of simulated time.
    // Step smaller than ClockPeriod to emulate loop polling.
    const uint32_t EndTimeUs = 100000;
    const uint32_t LoopStepUs = 50;

    while (SimulatedMicros < EndTimeUs) {
        LoopSimulation();
        SimulatedMicros += LoopStepUs;
    }

    return 0;
}
