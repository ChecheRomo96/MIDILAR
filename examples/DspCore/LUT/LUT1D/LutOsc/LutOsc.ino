#include <Arduino.h>
#include "MidilarSetup.h"
#include <MIDILAR_DspCore.h>

static constexpr size_t LutSize = 32;
static constexpr size_t BufferSize = 128;

static constexpr uint32_t ClockFreq = 1000000UL; // micros()
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

void FillBufferStep(uint32_t* Buffer, size_t& Position, bool& Ready);
uint32_t GenerateNextSample();

void DAC_Setup() {
    Serial.begin(115200);
}

void DAC_Callback(uint16_t value) {
    Serial.println(value);
}

void setup() {
    DAC_Setup();

    MIDILAR::DspCore::Generators::Periodic::SineLUT<uint32_t>(
        OscLUT,
        SignalRange / 2,
        SignalRange / 2,
        0
    );

    // Pre-fill both buffers before starting playback
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

uint32_t GenerateNextSample() {
    float Index = PhaseAccumulator * OscLUT.Size();

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

void loop() {
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

    // 2. Background work: refill inactive buffer incrementally
    if (ActiveBuffer) {
        // BufferB is playing, refill BufferA
        FillBufferStep(BufferA, FillPosition, BufferReadyA);
    } else {
        // BufferA is playing, refill BufferB
        FillBufferStep(BufferB, FillPosition, BufferReadyB);
    }
}