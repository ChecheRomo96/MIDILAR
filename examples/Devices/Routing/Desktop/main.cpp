#include "../Shared.h"
#include <iostream>
int main() {
    MIDILARExamples::Devices::Routing::Run([](const char* text) { std::cout << text; });
    return 0;
}
