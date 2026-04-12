/**
 * @file Methods.cpp
 * @brief Demonstrates binding and invoking instance methods using `CallbackHandler`.
 * @ingroup MIDILAR_Examples_CallbackHandler_Methods
 */

 #include <MIDILAR_SystemFoundation.h>
 #include <iostream>
 
 using namespace MIDILAR::SystemFoundation;
 
 // Test class with various method signatures
 class Printer {
 public:
     void PrintMsg(const char* msg) {
         std::cout << "[Printer::PrintMsg] " << msg << std::endl;
     }
 
     int GetValue() {
         std::cout << "[Printer::GetValue] Returning 7" << std::endl;
         return 7;
     }
 
     void SayHello() {
         std::cout << "[Printer::SayHello] Hello from instance!" << std::endl;
     }
 
     float Multiply(int a, float b) {
         float result = a * b;
         std::cout << "[Printer::Multiply] " << a << " * " << b << " = " << result << std::endl;
         return result;
     }
 };
 
 int main() {
     Printer p;
 
     // === Handlers for method binding ===
     CallbackHandler<void, const char*> handlerMsg;
     CallbackHandler<int, void> handlerInt;
     CallbackHandler<void, void> handlerVoid;
     CallbackHandler<float, int, float> handlerMath;
 
     // Bind instance methods
     handlerMsg.bind<Printer, &Printer::PrintMsg>(&p);
     handlerInt.bind<Printer, &Printer::GetValue>(&p);
     handlerVoid.bind<Printer, &Printer::SayHello>(&p);
     handlerMath.bind<Printer, &Printer::Multiply>(&p);
 
     // Invoke them
     handlerMsg.invoke("Test from method");
     
     int val = handlerInt.invoke();
     std::cout << "[handlerInt] Result = " << val << std::endl;
 
     handlerVoid.invoke();
 
     float product = handlerMath.invoke(5, 3.2f);
     std::cout << "[handlerMath] Returned: " << product << std::endl;
 
     return 0;
 }
 