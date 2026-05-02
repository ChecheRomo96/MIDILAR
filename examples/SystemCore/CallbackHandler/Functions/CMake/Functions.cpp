/**
 * @file Functions.cpp
 * @brief Demonstrates binding and invoking free functions using `CallbackHandler`.
 * @ingroup MIDILAR_Examples_CallbackHandler_Functions
 */

 #include <MIDILAR_SystemCore.h>
 #include <iostream>
 
 using namespace MIDILAR::SystemCore;
 
 // === Handlers for various function signatures ===
 CallbackHandler<void, const char*> handlerMsg;
 CallbackHandler<int, void> handlerInt;
 CallbackHandler<void, void> handlerVoid;
 CallbackHandler<float, int, float> handlerMath;
 
 // === Free-standing functions ===
 void PrintMessageA(const char* msg) {
     std::cout << "[A] " << msg << std::endl;
 }
 
 void PrintMessageB(const char* msg) {
     std::cout << "[B] " << msg << std::endl;
 }
 
 int Return42() {
     std::cout << "[Return42] Called" << std::endl;
     return 42;
 }
 
 void HelloVoid() {
     std::cout << "[HelloVoid] Hello from void()" << std::endl;
 }
 
 float Multiply(int a, float b) {
     std::cout << "[Multiply] " << a << " * " << b << " = " << a * b << std::endl;
     return a * b;
 }
 
 int main() {
     // --- Test void(const char*) ---
     handlerMsg.bind(PrintMessageA);
     handlerMsg.invoke("Hello from A!");
     handlerMsg.unbind();
     handlerMsg.bind(PrintMessageB);
     handlerMsg.invoke("Hello from B!");
 
     // --- Test int(void) ---
     handlerInt.bind(Return42);
     if (handlerInt.status()) {
         int result = handlerInt.invoke();
         std::cout << "[handlerInt] Result = " << result << std::endl;
     }
 
     // --- Test void(void) ---
     handlerVoid.bind(HelloVoid);
     if (handlerVoid.status()) {
         handlerVoid.invoke();
     }
 
     // --- Test float(int, float) ---
     handlerMath.bind(Multiply);
     if (handlerMath.status()) {
         float out = handlerMath.invoke(3, 2.5f);
         std::cout << "[handlerMath] Returned: " << out << std::endl;
     }
 
     return 0;
 }
 