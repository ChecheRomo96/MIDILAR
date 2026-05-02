#include "DeviceBase.h"

namespace MIDILAR::MidiCore {

    void DeviceBase::BindMidiOut(MidiOut_CallbackType MidiOutHandler) {
        _MidiOutHandler.bind(MidiOutHandler);
    }

    bool DeviceBase::MidiOutStatus() const {
        return _MidiOutHandler.status();
    }

    bool DeviceBase::HasCapability(Capabilities capability) const {
        return (_capabilities & static_cast<uint32_t>(capability)) != 0;
    }

    void DeviceBase::SetCapabilities(uint32_t capabilities) {
        _capabilities = capabilities;
    }

#if __has_include(<vector>)
    void DeviceBase::MidiInput(std::vector<uint8_t>::const_iterator begin,
                            std::vector<uint8_t>::const_iterator end)
    {
        if (begin == end) return;
        MidiInput(&(*begin), static_cast<size_t>(std::distance(begin, end)));
    }

    void DeviceBase::MidiOutput(std::vector<uint8_t>::const_iterator begin,
                            std::vector<uint8_t>::const_iterator end)
    {
        if (begin == end) return;
        MidiOutput(&(*begin), static_cast<size_t>(std::distance(begin, end)));
    }
#endif

    void DeviceBase::MidiInput(const uint8_t* Message, size_t size) {
        // Default implementation - derived classes can override
        (void)Message; // Prevent unused parameter warning
        (void)size;    // Prevent unused parameter warning
    }

    void DeviceBase::MidiOutput(const uint8_t* Message, size_t size) {
        _MidiOutHandler.invoke(Message, size);
    }

    void DeviceBase::MidiInput(const MidiCore::Message& message) {
        MidiInput(message.Buffer(), message.size());
    }

    void DeviceBase::MidiOutput(const MidiCore::Message& message) {
        MidiOutput(message.Buffer(), message.size());
    }

    void DeviceBase::Update(SystemCore::Clock::TimePoint SystemTime) {
        // Default implementation - derived classes can override
        (void)SystemTime; // Prevent unused parameter warning
    }

    void DeviceBase::ClockTick() {
        // Default implementation - derived classes can override
    }

} // namespace MIDILAR
