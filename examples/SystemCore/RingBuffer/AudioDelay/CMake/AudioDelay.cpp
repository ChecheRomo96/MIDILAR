/**
 * @file AudioDelay.cpp
 * @brief Demonstrates RingBuffer as an audio-style delay line.
 * @ingroup MIDILAR_Examples_SystemCore_RingBuffer_AudioDelay
 */

#include <MIDILAR_SystemCore.h>
#include <iostream>

using namespace MIDILAR::SystemCore;

static float ReadFc(size_t n)
{
    return 220.0f + static_cast<float>(n) * 10.0f;
}

static float ProcessOscillator(float fc)
{
    return fc * 0.001f;
}

int main()
{
    std::cout << "== RingBuffer AudioDelay ==\n";

    const size_t DelayLength = 8;
    float audioMemory[DelayLength];
    RingBuffer<float> audioBuffer(audioMemory, DelayLength);

    for (size_t i = 0; i < DelayLength; ++i)
    {
        audioBuffer.Push(0.0f);
    }

    for (size_t n = 0; n < 16; ++n)
    {
        float output = 0.0f;
        audioBuffer.Pop(output);

        float fc = ReadFc(n);
        float newSample = ProcessOscillator(fc);
        audioBuffer.Push(newSample);

        std::cout << "n=" << n
                  << " output=" << output
                  << " fc=" << fc
                  << " newSample=" << newSample
                  << "\n";
    }

    return 0;
}
