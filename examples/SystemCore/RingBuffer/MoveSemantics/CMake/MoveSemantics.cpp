/**
 * @file MoveSemantics.cpp
 * @brief Demonstrates RingBuffer move semantics with a non-copyable object.
 * @ingroup MIDILAR_Examples_SystemCore_RingBuffer_MoveSemantics
 */

#include <MIDILAR_SystemCore.h>
#include <iostream>
#include <utility>

using namespace MIDILAR::SystemCore;

struct AudioBlock
{
    float* data;
    size_t size;

    AudioBlock()
        : data(nullptr), size(0)
    {
    }

    AudioBlock(float* inputData, size_t inputSize)
        : data(inputData), size(inputSize)
    {
    }

    AudioBlock(const AudioBlock&) = delete;
    AudioBlock& operator=(const AudioBlock&) = delete;

    AudioBlock(AudioBlock&& other) noexcept
        : data(other.data), size(other.size)
    {
        other.data = nullptr;
        other.size = 0;
    }

    AudioBlock& operator=(AudioBlock&& other) noexcept
    {
        if (this != &other)
        {
            data = other.data;
            size = other.size;

            other.data = nullptr;
            other.size = 0;
        }

        return *this;
    }
};

int main()
{
    std::cout << "== RingBuffer MoveSemantics ==\n";

    float blockAData[4] = {0.10f, 0.20f, 0.30f, 0.40f};
    float blockBData[8] = {0.50f, 0.60f, 0.70f, 0.80f, 0.90f, 1.00f, 1.10f, 1.20f};

    AudioBlock memory[2];
    RingBuffer<AudioBlock> rb(memory, 2);

    AudioBlock blockA(blockAData, 4);
    AudioBlock blockB(blockBData, 8);

    rb.Push(std::move(blockA));
    rb.Push(std::move(blockB));

    AudioBlock out;
    while (rb.Pop(out))
    {
        std::cout << "Block size=" << out.size << " data:";
        for (size_t i = 0; i < out.size; ++i)
        {
            std::cout << " " << out.data[i];
        }
        std::cout << "\n";
    }

    return 0;
}
