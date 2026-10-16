#include "../Shared.h"
#include <iostream>
int main() {
    MIDILARExamples::Protocol::Midi1Messages::Run([](const char* text) { std::cout << text; });
    return 0;
}
