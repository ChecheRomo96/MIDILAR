/**
 * @file BasicFloat.ino
 * @brief Arduino example showing basic RingBuffer usage with float values.
 * @ingroup MIDILAR_Examples_SystemCore_RingBuffer_BasicFloat
 */

#include <MIDILAR_SystemCore.h>

using namespace MIDILAR::SystemCore;

float memory[4];
RingBuffer<float> rb(memory, 4);

void setup()
{
    Serial.begin(115200);

    rb.Push(0.25f);
    rb.Push(0.50f);
    rb.Push(0.75f);

    Serial.print("Available: ");
    Serial.println(rb.GetAvailable());

    float value = 0.0f;
    while (rb.Pop(value))
    {
        Serial.print("Pop: ");
        Serial.println(value, 4);
    }
}

void loop()
{
}
