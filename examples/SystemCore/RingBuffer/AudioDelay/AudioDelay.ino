/**
 * @file AudioDelay.ino
 * @brief Arduino example showing RingBuffer as an audio-style delay line.
 * @ingroup MIDILAR_Examples_SystemCore_RingBuffer_AudioDelay
 */

#include <MIDILAR_SystemCore.h>

using namespace MIDILAR::SystemCore;

const size_t DelayLength = 8;
float audioMemory[DelayLength];
RingBuffer<float> audioBuffer(audioMemory, DelayLength);
size_t n = 0;

float ReadFc()
{
    return 220.0f + static_cast<float>(n) * 10.0f;
}

float ProcessOscillator(float fc)
{
    return fc * 0.001f;
}

void setup()
{
    Serial.begin(115200);

    for (size_t i = 0; i < DelayLength; ++i)
    {
        audioBuffer.Push(0.0f);
    }
}

void loop()
{
    float output = 0.0f;
    audioBuffer.Pop(output);

    float fc = ReadFc();
    float newSample = ProcessOscillator(fc);
    audioBuffer.Push(newSample);

    Serial.print("n=");
    Serial.print(n);
    Serial.print(" output=");
    Serial.print(output, 4);
    Serial.print(" fc=");
    Serial.print(fc, 2);
    Serial.print(" newSample=");
    Serial.println(newSample, 4);

    n++;
    delay(250);
}
