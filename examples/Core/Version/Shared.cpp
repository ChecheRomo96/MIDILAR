#include "Shared.h"

#include <MIDILAR/Core/Version.h>
#include <CPSTL_BuildSettings.h>
#include <DspCore_BuildSettings.h>

namespace MIDILARExamples::Core::Version {

const char* MIDILARVersion() noexcept {
    return MIDILAR::Core::Version();
}

const char* FoundationVersion() noexcept {
    return MIDILAR::Core::FoundationVersion();
}

const char* MCCVersion() noexcept {
    return MIDILAR::Core::MCCVersion();
}

const char* DspCoreVersion() noexcept {
    return DSPCORE_VERSION;
}

const char* CPSTLVersion() noexcept {
    return CPSTL_VERSION;
}

void Run(PrintFunction print) noexcept {
    // Read versions from the library and dependency headers, then render the
    // complete chain through the same callback on desktop and Arduino.
    print("========================================\n");
    print(" MIDILAR :: Core / Version\n");
    print("========================================\n");
    print(" MIDILAR ....... ");
    print(MIDILARVersion());
    print("\n MCC ........... ");
    print(MCCVersion());
    print("\n DspCore ....... ");
    print(DspCoreVersion());
    print("\n Foundation .... ");
    print(FoundationVersion());
    print("\n CPSTL ......... ");
    print(CPSTLVersion());
    print("\n========================================\n");
}

} // namespace MIDILARExamples::Core::Version
