/**
 * @file StructFrame.cpp
 * @brief Demonstrates RingBuffer usage with a custom frame struct.
 * @ingroup MIDILAR_Examples_SystemCore_RingBuffer_StructFrame
 */

#include <MIDILAR_SystemCore.h>
#include <iostream>

using namespace MIDILAR::SystemCore;

struct AudioFrame
{
    float sample;
    float fc;
    float phase;
};

int main()
{
    std::cout << "== RingBuffer StructFrame ==\n";

    AudioFrame memory[4];
    RingBuffer<AudioFrame> rb(memory, 4);

    rb.Push({0.10f, 220.0f, 0.00f});
    rb.Push({0.20f, 330.0f, 0.25f});
    rb.Push({0.30f, 440.0f, 0.50f});

    AudioFrame frame;
    while (rb.Pop(frame))
    {
        std::cout << "sample=" << frame.sample
                  << " fc=" << frame.fc
                  << " phase=" << frame.phase
                  << "\n";
    }

    return 0;
}
