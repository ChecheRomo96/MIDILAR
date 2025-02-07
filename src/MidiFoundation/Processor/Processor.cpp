#include "Processor.h"

namespace MIDILAR::MidiFoundation {

    void Processor::BindMidiOut(MidiOut_CallbackReferenceType MidiOutHandler) {
        _MidiOutHandler.bind(MidiOutHandler);
    }

    bool Processor::MidiOutStatus() const {
        return _MidiOutHandler.status();
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
        if (begin == end) return;
        MidiInput( reinterpret_cast<uint8_t*>(&(*begin)), std::distance(begin, end));
    }

    void Processor::MidiOutput(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end) {
        if (begin == end) return;
        MidiOutput( reinterpret_cast<uint8_t*>(&(*begin)), std::distance(begin, end));
    }
#endif

    void Processor::MidiInput(const uint8_t* Message, size_t size) {
        // Default implementation - derived classes can override
        (void)Message; // Prevent unused parameter warning
        (void)size;    // Prevent unused parameter warning
    }

    void Processor::MidiOutput(const uint8_t* Message, size_t size) {
        _MidiOutHandler.invoke(Message, size);
    }

    void Processor::MidiInput(const MidiFoundation::Message& message) {
        MidiInput(message.Buffer(), message.size());
    }

    void Processor::MidiOutput(const MidiFoundation::Message& message) {
        MidiOutput(message.Buffer(), message.size());
    }

    void Processor::Update(SystemFoundation::Clock::TimePoint SystemTime) {
        // Default implementation - derived classes can override
        (void)SystemTime; // Prevent unused parameter warning
    }

    void Processor::ClockTick() {
        // Default implementation - derived classes can override
    }

} // namespace MIDILAR
