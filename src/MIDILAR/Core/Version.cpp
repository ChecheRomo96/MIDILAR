#include <MIDILAR/Core/Version.h>

#include <Foundation_BuildSettings.h>
#include <MCC_BuildSettings.h>
#include <MIDILAR_BuildSettings.h>

namespace MIDILAR::Core {

const char* Version() noexcept {
    return MIDILAR_VERSION;
}

const char* FoundationVersion() noexcept {
    return FOUNDATION_VERSION;
}

const char* MCCVersion() noexcept {
    return MCC_VERSION;
}

} // namespace MIDILAR::Core
