/**
 * @file MoveSemantics.ino
 * @brief Arduino example showing RingBuffer move semantics with a non-copyable object.
 * @ingroup MIDILAR_Examples_SystemCore_RingBuffer_MoveSemantics
 */

#include <MIDILAR_SystemCore.h>
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

float blockAData[4] = {0.10f, 0.20f, 0.30f, 0.40f};
float blockBData[4] = {0.50f, 0.60f, 0.70f, 0.80f};
AudioBlock memory[2];
RingBuffer<AudioBlock> rb(memory, 2);

void setup()
{
    Serial.begin(115200);

    AudioBlock blockA(blockAData, 4);
    AudioBlock blockB(blockBData, 4);

    rb.Push(std::move(blockA));
    rb.Push(std::move(blockB));

    Serial.print("blockA after move size=");
    Serial.println(blockA.size);

    AudioBlock out;
    while (rb.Pop(out))
    {
        Serial.print("Block size=");
        Serial.print(out.size);
        Serial.print(" data:");

        for (size_t i = 0; i < out.size; ++i)
        {
            Serial.print(" ");
            Serial.print(out.data[i], 4);
        }

        Serial.println();
    }
}

void loop()
{
}
