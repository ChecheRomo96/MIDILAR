#include "../Shared.h"
#include <iostream>
int main() {
    MIDILARExamples::Protocol::CreatingMessages::Run([](const char* text) { std::cout << text; });
    return 0;
}
