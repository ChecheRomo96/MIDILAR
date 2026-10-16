#include "../Shared.h"
#include <iostream>
int main() {
    MIDILARExamples::Devices::HeldNotes::Run([](const char* text) { std::cout << text; });
    return 0;
}
