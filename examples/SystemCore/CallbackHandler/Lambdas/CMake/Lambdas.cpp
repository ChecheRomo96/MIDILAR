/**
 * @file Lambdas.cpp
 * @brief Demonstrates binding lambda expressions using `CallbackHandler`.
 * @ingroup MIDILAR_Examples_CallbackHandler_Lambdas
 */

 #include <MIDILAR_SystemCore.h>
 #include <iostream>
 
 using namespace MIDILAR::SystemCore;
 
 // === Global handler instances with different signatures ===
 CallbackHandler<void, const char*> handlerMsg;
 CallbackHandler<int, void> handlerInt;
 CallbackHandler<void, void> handlerVoid;
 CallbackHandler<float, int, float> handlerMath;
 
 int main() {
     std::cout << "== CallbackHandler Lambda Binding ==\n";
 
     // --- Lambda: void(const char*) ---
     handlerMsg.bind([](const char* msg) {
         std::cout << "[Lambda Message] " << msg << std::endl;
     });
     handlerMsg.invoke("Hello from lambda!");
 
     // --- Lambda: int(void) ---
     handlerInt.bind([]() -> int {
         std::cout << "[Lambda Int] Returning 99\n";
         return 99;
     });
     int result = handlerInt.invoke();
     std::cout << "[handlerInt] Result = " << result << std::endl;
 
     // --- Lambda: void(void) ---
     handlerVoid.bind([]() {
         std::cout << "[Lambda Void] Simple call\n";
     });
     handlerVoid.invoke();
 
     // --- Lambda: float(int, float) ---
     handlerMath.bind([](int a, float b) -> float {
         float result = a * b;
         std::cout << "[Lambda Multiply] " << a << " * " << b << " = " << result << std::endl;
         return result;
     });
     float f = handlerMath.invoke(2, 3.5f);
     std::cout << "[handlerMath] Returned: " << f << std::endl;
 
     return 0;
 }
 