#ifndef MIDILAR_CORE_VERSION_H
#define MIDILAR_CORE_VERSION_H

namespace MIDILAR::Core {

/** @brief Returns the MIDILAR semantic version compiled into the library. */
const char* Version() noexcept;

/** @brief Returns the Foundation semantic version used to build MIDILAR. */
const char* FoundationVersion() noexcept;

/**
 * @brief Returns the MCC version this build of MIDILAR was compiled against.
 * @ingroup MIDILAR_Core
 */
const char* MCCVersion() noexcept;

} // namespace MIDILAR::Core

#endif // MIDILAR_CORE_VERSION_H
