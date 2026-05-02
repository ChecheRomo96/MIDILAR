/**
 * @file Lambdas.ino
 * @brief Demonstrates binding lambda expressions using `CallbackHandler` in Arduino.
 * @ingroup MIDILAR_Examples_CallbackHandler_Lambdas
 */

#include <MIDILAR_SystemCore.h>

using namespace MIDILAR::SystemCore;

// Handlers with different signatures
CallbackHandler<void, const char*> handlerMsg;
CallbackHandler<int, void> handlerInt;
CallbackHandler<void, void> handlerVoid;
CallbackHandler<float, int, float> handlerMath;

void setup() {
    Serial.begin(115200);
    while (!Serial);  // Wait for USB Serial

    Serial.println(F("== CallbackHandler Lambda Binding (Arduino) =="));

    // --- Lambda: void(const char*) ---
    handlerMsg.bind([](const char* msg) {
        Serial.print(F("[Lambda Message] "));
        Serial.println(msg);
    });
    handlerMsg.invoke("Hello from lambda!");

    // --- Lambda: int(void) ---
    handlerInt.bind([]() -> int {
        Serial.println(F("[Lambda Int] Returning 99"));
        return 99;
    });
    int val = handlerInt.invoke();
    Serial.print(F("[handlerInt] Result = "));
    Serial.println(val);

    // --- Lambda: void(void) ---
    handlerVoid.bind([]() {
        Serial.println(F("[Lambda Void] Called"));
    });
    handlerVoid.invoke();

    // --- Lambda: float(int, float) ---
    handlerMath.bind([](int a, float b) -> float {
        float result = a * b;
        Serial.print(F("[Lambda Multiply] "));
        Serial.println(result);
        return result;
    });
    float product = handlerMath.invoke(2, 3.5f);
    Serial.print(F("[handlerMath] Returned = "));
    Serial.println(product);
}

void loop() {
    // No repeated logic
}
