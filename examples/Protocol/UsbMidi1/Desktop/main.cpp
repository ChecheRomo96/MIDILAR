#include "../Shared.h"
#include <iostream>
int main() {
    MIDILARExamples::Protocol::UsbMidi1::Run([](const char* text) { std::cout << text; });
    return 0;
}
