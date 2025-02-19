#include <iostream>
#include <cstdint>
#include <SystemFoundation/CallbackHandler/CallbackHandler.h>

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
    std::cout << "Callback (UpdateAndReturn): " << (int)x << std::endl;
    globalValue = x;
    return globalValue;
}

/**
 * @brief  Updates global value but does not return anything.
 * @param  x: Input value
 */
void UpdateOnly(uint8_t x) {
    std::cout << "Callback (UpdateOnly): " << (int)x << std::endl;
    globalValue = x;
}

/**
 * @brief  Returns the global value and increments it.
 * @return Updated value
 */
uint8_t IncrementAndReturn() {
    std::cout << "Callback (IncrementAndReturn): " << (int)++globalValue << std::endl;
    return globalValue;
}

/**
 * @brief  Prints the current global value.
 */
void PrintGlobalValue() {
    std::cout << "Callback (PrintGlobalValue): " << (int)globalValue << std::endl;
}

// ===============================
// Main Execution
// ===============================
int main() {
    // Bind callback functions to handlers
    CallbackReturnsUint8.bind(UpdateAndReturn);
    CallbackTakesUint8.bind(UpdateOnly);
    CallbackReturnsUint8Only.bind(IncrementAndReturn);
    CallbackNoArgs.bind(PrintGlobalValue);

    // ===============================
    // Test Case 1: Using CallbackReturnsUint8
    // ===============================
    uint8_t returnedValue;
    std::cout << "\n--- Test 1: CallbackReturnsUint8 ---\n";
    for (uint8_t i = 0; i < 10; i++) {
        returnedValue = CallbackReturnsUint8.invoke(i);
        std::cout << "Invoked with: " << (int)i << " -> Returned: " << (int)returnedValue << std::endl;
    }

    // ===============================
    // Test Case 2: Using CallbackTakesUint8
    // ===============================
    std::cout << "\n--- Test 2: CallbackTakesUint8 ---\n";
    for (uint8_t i = 0; i < 10; i++) {
        CallbackTakesUint8.invoke(i);
    }

    // ===============================
    // Test Case 3: Using CallbackReturnsUint8Only
    // ===============================
    std::cout << "\n--- Test 3: CallbackReturnsUint8Only ---\n";
    for (uint8_t i = 0; i < 5; i++) {
        CallbackReturnsUint8Only.invoke();
    }

    // ===============================
    // Test Case 4: Using CallbackNoArgs
    // ===============================
    std::cout << "\n--- Test 4: CallbackNoArgs ---\n";
    CallbackNoArgs.invoke();

    return 0;
}
