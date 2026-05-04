/**
 * @file StructFrame.ino
 * @brief Arduino example showing RingBuffer usage with a custom frame struct.
 * @ingroup MIDILAR_Examples_SystemCore_RingBuffer_StructFrame
 */

#include <MIDILAR_SystemCore.h>

using namespace MIDILAR::SystemCore;

struct AudioFrame
{
    float sample;
    float fc;
    float phase;
};

AudioFrame memory[4];
RingBuffer<AudioFrame> rb(memory, 4);

void setup()
{
    Serial.begin(115200);

    rb.Push({0.10f, 220.0f, 0.00f});
    rb.Push({0.20f, 330.0f, 0.25f});
    rb.Push({0.30f, 440.0f, 0.50f});

    AudioFrame frame;
    while (rb.Pop(frame))
    {
        Serial.print("sample=");
        Serial.print(frame.sample, 4);
        Serial.print(" fc=");
        Serial.print(frame.fc, 2);
        Serial.print(" phase=");
        Serial.println(frame.phase, 4);
    }
}

void loop()
{
}
