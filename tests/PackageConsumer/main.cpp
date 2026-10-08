#include <Foundation/Math/Arithmetic.h>
#include <MCC.h>
#include <MIDILAR.h>

#include <cstring>
#include <iostream>

#ifndef MIDILAR_CORE
    #error "The installed MIDILAR package must export MIDILAR_CORE"
#endif

int main() {
    // The installed package must bring its transitive Foundation, DspCore and MCC
    // targets with it.
    const auto gcd = Foundation::Math::GCD(12, 8);
    constexpr MCC::NoteName cSharp(MCC::Letter::C, MCC::Accidental::Sharp());

    std::cout << "MIDILAR: " << MIDILAR::Core::Version() << '\n';
    std::cout << "MCC: " << MIDILAR::Core::MCCVersion() << '\n';
    std::cout << "Foundation: " << MIDILAR::Core::FoundationVersion() << '\n';
    std::cout << "Foundation::Math::GCD(12, 8): " << gcd << '\n';
    std::cout << "C# pitch class: " << static_cast<int>(cSharp.PitchClass().Value()) << '\n';

    return (gcd == 4U && cSharp.PitchClass().Value() == 1 &&
            std::strcmp(MIDILAR::Core::Version(), MIDILAR_VERSION) == 0) ? 0 : 1;
}
