#include "../Shared.h"

#include <iostream>

int main() {
    std::cout << "========================================\n"
              << " MIDILAR :: Core / Version\n"
              << "========================================\n"
              << " MIDILAR ....... "
              << MIDILARExamples::Core::Version::MIDILARVersion() << '\n'
              << " MCC ........... "
              << MIDILARExamples::Core::Version::MCCVersion() << '\n'
              << " Foundation .... "
              << MIDILARExamples::Core::Version::FoundationVersion() << '\n'
              << "========================================\n";
    return 0;
}
