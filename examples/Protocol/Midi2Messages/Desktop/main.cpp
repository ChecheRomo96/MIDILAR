#include "../Shared.h"
#include <iostream>
int main() {
    MIDILARExamples::Protocol::Midi2Messages::Run([](const char* text) { std::cout << text; });
    return 0;
}
