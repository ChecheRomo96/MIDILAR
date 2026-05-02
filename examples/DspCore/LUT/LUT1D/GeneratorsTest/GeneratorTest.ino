#include <MIDILAR.h>

using namespace MIDILAR::DspCore::Generators::Periodic;

#define SIZE 8

float buffer[SIZE];

void PrintBuffer(const char* name) {
    Serial.println(name);
    for (size_t i = 0; i < SIZE; ++i) {
        Serial.print(buffer[i], 4);
        Serial.print(" ");
    }
    Serial.println();
    Serial.println();
}

void setup() {
    Serial.begin(115200);

    Serial.println("=== Generator Tests ===");

    Sine<float>(buffer, SIZE, 1.0f, 0.0f, 0.0f);
    PrintBuffer("Sine");

    Square<float>(buffer, SIZE, 1.0f, 0.0f, 0.0f);
    PrintBuffer("Square");

    Triangle<float>(buffer, SIZE, 1.0f, 0.0f, 0.0f);
    PrintBuffer("Triangle");

    Saw<float>(buffer, SIZE, 1.0f, 0.0f, 0.0f);
    PrintBuffer("Saw");
}

void loop() {}