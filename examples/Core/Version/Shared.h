#ifndef MIDILAR_EXAMPLES_CORE_VERSION_SHARED_H
#define MIDILAR_EXAMPLES_CORE_VERSION_SHARED_H

namespace MIDILARExamples::Core::Version {

const char* MIDILARVersion() noexcept;
const char* FoundationVersion() noexcept;
const char* MCCVersion() noexcept;
const char* DspCoreVersion() noexcept;
const char* CPSTLVersion() noexcept;

using PrintFunction = void (*)(const char* text);
void Run(PrintFunction print) noexcept;

} // namespace MIDILARExamples::Core::Version

#endif // MIDILAR_EXAMPLES_CORE_VERSION_SHARED_H
