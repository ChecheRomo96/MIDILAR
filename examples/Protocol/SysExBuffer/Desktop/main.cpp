#include "../Shared.h"
#include <iostream>
int main() {
    MIDILARExamples::Protocol::SysExBuffer::Run([](const char* text) { std::cout << text; });
    return 0;
}
