#include "../Shared.h"

#include <iostream>

int main() {
    MIDILARExamples::Core::Version::Run(
        [](const char* text) { std::cout << text; });
    return 0;
}
