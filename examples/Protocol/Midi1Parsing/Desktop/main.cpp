#include "../Shared.h"
#include <iostream>
int main() {
    MIDILARExamples::Protocol::Midi1Parsing::Run([](const char* text) { std::cout << text; });
    return 0;
}
