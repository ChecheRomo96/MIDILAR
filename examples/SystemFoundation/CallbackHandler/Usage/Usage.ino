#include <Arduino.h>
#include <MIDILAR_SystemFoundation.h>  // Assuming a similar library exists for Arduino

using namespace MIDILAR::SystemFoundation;

// ===============================
// Callback Handlers
// ===============================
CallbackHandler<uint8_t, uint8_t>   CallbackReturnsUint8;   // Function takes and returns uint8_t
CallbackHandler<void, uint8_t>      CallbackTakesUint8;     // Function takes uint8_t, returns void
CallbackHandler<uint8_t, void>      CallbackReturnsUint8Only; // Function takes void, returns uint8_t
CallbackHandler<void, void>         CallbackNoArgs;         // Function takes and returns void

// ===============================
// Global State Variable
// ===============================
uint8_t globalValue = 0;

// ===============================
// Callback Functions
// ===============================

/**
 * @brief  Updates global value and returns it.
 * @param  x: Input value
 * @return Updated value
 */
uint8_t UpdateAndReturn(uint8_t x) {
    Serial.print("Callback (UpdateAndReturn): ");
    Serial.println(x);
    globalValue = x;
    return globalValue;
}

/**
 * @brief  Updates global value but does not return anything.
 * @param  x: Input value
 */
void UpdateOnly(uint8_t x) {
    Serial.print("Callback (UpdateOnly): ");
    Serial.println(x);
    globalValue = x;
}

/**
 * @brief  Returns the global value and increments it.
 * @return Updated value
 */
uint8_t IncrementAndReturn() {
    Serial.print("Callback (IncrementAndReturn): ");
    Serial.println(++globalValue);
    return globalValue;
}

/**
 * @brief  Prints the current global value.
 */
void PrintGlobalValue() {
    Serial.print("Callback (PrintGlobalValue): ");
    Serial.println(globalValue);
}

// ===============================
// Arduino Setup & Loop
// ===============================

void setup() {
    Serial.begin(115200);  // Initialize Serial Monitor

    // Bind callback functions to handlers
    CallbackReturnsUint8.bind(UpdateAndReturn);
    CallbackTakesUint8.bind(UpdateOnly);
    CallbackReturnsUint8Only.bind(IncrementAndReturn);
    CallbackNoArgs.bind(PrintGlobalValue);

    Serial.println("\n=== Arduino Callback Test ===\n");

    // ===============================
    // Test Case 1: Using CallbackReturnsUint8
    // ===============================
    Serial.println("\n--- Test 1: CallbackReturnsUint8 ---");
    for (uint8_t i = 0; i < 10; i++) {
        uint8_t returnedValue = CallbackReturnsUint8.invoke(i);
        Serial.print("Invoked with: ");
        Serial.print(i);
        Serial.print(" -> Returned: ");
        Serial.println(returnedValue);
    }

    // ===============================
    // Test Case 2: Using CallbackTakesUint8
    // ===============================
    Serial.println("\n--- Test 2: CallbackTakesUint8 ---");
    for (uint8_t i = 0; i < 10; i++) {
        CallbackTakesUint8.invoke(i);
    }

    // ===============================
    // Test Case 3: Using CallbackReturnsUint8Only
    // ===============================
    Serial.println("\n--- Test 3: CallbackReturnsUint8Only ---");
    for (uint8_t i = 0; i < 5; i++) {
        CallbackReturnsUint8Only.invoke();
    }

    // ===============================
    // Test Case 4: Using CallbackNoArgs
    // ===============================
    Serial.println("\n--- Test 4: CallbackNoArgs ---");
    CallbackNoArgs.invoke();
}

void loop() {
    // Nothing here, since the tests run in setup()
}
