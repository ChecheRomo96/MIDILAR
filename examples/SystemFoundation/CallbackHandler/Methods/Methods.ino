/**
 * @file Methods.ino
 * @brief Arduino-compatible example showing instance method binding with `CallbackHandler`.
 * @ingroup MIDILAR_Examples_CallbackHandler_Methods
 */

#include <MIDILAR_SystemFoundation.h>

using namespace MIDILAR::SystemFoundation;

// === Printer class with various method signatures ===
class Printer {
public:
    void PrintMsg(const char* msg) {
        Serial.print(F("[PrintMsg] "));
        Serial.println(msg);
    }

    int GetValue() {
        Serial.println(F("[GetValue] Returning 123"));
        return 123;
    }

    void SayHello() {
        Serial.println(F("[SayHello] Hello from instance!"));
    }

    float Multiply(int a, float b) {
        float result = a * b;
        Serial.print(F("[Multiply] "));
        Serial.println(result);
        return result;
    }
};

// === Instance of class and handlers ===
Printer printer;

CallbackHandler<void, const char*> handlerMsg;
CallbackHandler<int, void> handlerInt;
CallbackHandler<void, void> handlerVoid;
CallbackHandler<float, int, float> handlerMath;

void setup() {
    Serial.begin(115200);
    while (!Serial);  // Wait for USB Serial on some boards

    Serial.println(F("== MIDILAR CallbackHandler: Method Binding =="));

    // Bind each method
    handlerMsg.bind<Printer, &Printer::PrintMsg>(&printer);
    handlerInt.bind<Printer, &Printer::GetValue>(&printer);
    handlerVoid.bind<Printer, &Printer::SayHello>(&printer);
    handlerMath.bind<Printer, &Printer::Multiply>(&printer);

    // Invoke handlers
    handlerMsg.invoke("Hello from Arduino!");
    
    int value = handlerInt.invoke();
    Serial.print(F("[handlerInt] Result = "));
    Serial.println(value);

    handlerVoid.invoke();

    float result = handlerMath.invoke(3, 2.5f);
    Serial.print(F("[handlerMath] Returned = "));
    Serial.println(result);
}

void loop() {
    // No repeated actions in this example
}
