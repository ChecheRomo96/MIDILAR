#include "MidiProcessor.h"

namespace MIDILAR {

    void MidiProcessor::MidiOutApiLink(MidiOutEventType MidiOutHandler) {
        _MidiOutHandler = MidiOutHandler;
    }

    bool MidiProcessor::MidiOutApiStatus() const {
        return _MidiOutHandler != nullptr;
    }

    bool MidiProcessor::HasCapability(Capabilities capability) const {
        return (_capabilities & static_cast<uint32_t>(capability)) != 0;
    }

    void MidiProcessor::SetCapabilities(uint32_t capabilities) {
        _capabilities = capabilities;
    }

#if __has_include(<vector>)
    void MidiProcessor::MidiInput(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end) {
        // Pass vector data to raw MIDI input handler
        MidiInput(&(*begin), std::distance(begin, end));
    }

    void MidiProcessor::MidiOutput(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end) {
        if (_MidiOutHandler) {
            _MidiOutHandler(&(*begin), std::distance(begin, end));
        }
    }
#endif

    void MidiProcessor::MidiInput(uint8_t* Message, size_t size) {
        // Default implementation - derived classes can override
        (void)Message; // Prevent unused parameter warning
        (void)size;    // Prevent unused parameter warning
    }

    void MidiProcessor::MidiOutput(uint8_t* Message, size_t size) {
        if (_MidiOutHandler) {
            _MidiOutHandler(Message, size);
        }
    }

    void MidiProcessor::Update(SystemTools::Clock::TimePoint SystemTime) {
        // Default implementation - derived classes can override
        (void)SystemTime; // Prevent unused parameter warning
    }

    void MidiProcessor::ClockTick() {
        // Default implementation - derived classes can override
    }

} // namespace MIDILAR
