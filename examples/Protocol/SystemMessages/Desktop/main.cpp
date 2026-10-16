#include "../Shared.h"
#include <iostream>
int main() {
    MIDILARExamples::Protocol::SystemMessages::Run([](const char* text) { std::cout << text; });
    return 0;
}
