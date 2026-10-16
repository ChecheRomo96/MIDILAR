#include "../Shared.h"
#include <iostream>
int main() {
    MIDILARExamples::Protocol::Midi1Encoding::Run([](const char* text) { std::cout << text; });
    return 0;
}
