/**
 * @file BasicFloat.cpp
 * @brief Demonstrates basic RingBuffer usage with float values.
 * @ingroup MIDILAR_Examples_SystemCore_RingBuffer_BasicFloat
 */

#include <MIDILAR_SystemCore.h>
#include <iostream>

using namespace MIDILAR::SystemCore;

int main()
{
    std::cout << "== RingBuffer BasicFloat ==\n";

    float memory[4];
    RingBuffer<float> rb(memory, 4);

    rb.Push(0.25f);
    rb.Push(0.50f);
    rb.Push(0.75f);

    std::cout << "Available: " << rb.GetAvailable() << "\n";
    std::cout << "FreeSpace: " << rb.GetFreeSpace() << "\n";

    float value = 0.0f;
    while (rb.Pop(value))
    {
        std::cout << "Pop: " << value << "\n";
    }

    std::cout << "Empty: " << rb.IsEmpty() << "\n";

    return 0;
}
