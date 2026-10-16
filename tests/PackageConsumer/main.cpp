#include <Foundation/Math/Arithmetic.h>
#include <MCC.h>
#include <MIDILAR.h>
#include <MIDILAR/Protocol/Midi1/Message.h>
#include <MIDILAR/Protocol/Midi1/SysExMessage.h>

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

    MIDILAR::Protocol::Midi1::Message message;
    message.NoteOn(60, 100, 0);
    uint8_t sysexStorage[4];
    const uint8_t payload[] = {0x7D, 1};
    const auto sysex = MIDILAR::Protocol::Midi1::SysExMessage::Build(
        sysexStorage, sizeof(sysexStorage), payload, sizeof(payload));

    std::cout << "MIDILAR: " << MIDILAR::Core::Version() << '\n';
    std::cout << "MCC: " << MIDILAR::Core::MCCVersion() << '\n';
    std::cout << "Foundation: " << MIDILAR::Core::FoundationVersion() << '\n';
    std::cout << "Foundation::Math::GCD(12, 8): " << gcd << '\n';
    std::cout << "C# pitch class: " << static_cast<int>(cSharp.PitchClass().Value()) << '\n';

    return (message.IsValid() && message.size() == 3 && message.Data(0) == 0x90 &&
            sysex.IsValid() && sysex.size() == 4 && sysex.Data(3) == 0xF7 &&
            gcd == 4U && cSharp.PitchClass().Value() == 1 &&
            std::strcmp(MIDILAR::Core::Version(), MIDILAR_VERSION) == 0) ? 0 : 1;
}
