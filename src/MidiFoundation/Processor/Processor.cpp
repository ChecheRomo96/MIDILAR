#include "Processor.h"

namespace MIDILAR::MidiFoundation {

    void Processor::MidiOutApiLink(MidiOutEventType MidiOutHandler) {
        _MidiOutHandler = MidiOutHandler;
    }

    bool Processor::MidiOutApiStatus() const {
        return _MidiOutHandler != nullptr;
    }

    bool Processor::HasCapability(Capabilities capability) const {
        return (_capabilities & static_cast<uint32_t>(capability)) != 0;
    }

    void Processor::SetCapabilities(uint32_t capabilities) {
        _capabilities = capabilities;
    }

#if __has_include(<vector>)
    void Processor::MidiInput(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end) {
        // Pass vector data to raw MIDI input handler
        MidiInput(&(*begin), std::distance(begin, end));
    }

    void Processor::MidiOutput(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end) {
        if (_MidiOutHandler) {
            _MidiOutHandler(&(*begin), std::distance(begin, end));
        }
    }
#endif

    void Processor::MidiInput(uint8_t* Message, size_t size) {
        // Default implementation - derived classes can override
        (void)Message; // Prevent unused parameter warning
        (void)size;    // Prevent unused parameter warning
    }

    void Processor::MidiOutput(uint8_t* Message, size_t size) {
        if (_MidiOutHandler) {
            _MidiOutHandler(Message, size);
        }
    }

    void Processor::Update(SystemFoundation::Clock::TimePoint SystemTime) {
        // Default implementation - derived classes can override
        (void)SystemTime; // Prevent unused parameter warning
    }

    void Processor::ClockTick() {
        // Default implementation - derived classes can override
    }

} // namespace MIDILAR
