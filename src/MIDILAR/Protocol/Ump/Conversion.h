#ifndef MIDILAR_PROTOCOL_UMP_CONVERSION_H
#define MIDILAR_PROTOCOL_UMP_CONVERSION_H
#include <MIDILAR_BuildSettings.h>
#include "Packet.h"
#include "../Midi/Message.h"
#include "../Midi2/Message.h"

namespace MIDILAR::Protocol::Ump {

/** @brief Encodes native short MIDI 1.0, normalizing velocity-zero Note On. @ingroup MIDILAR_Ump */
inline Packet Encode(Group group, const Midi::Message& message) noexcept {
    if (!message.IsValid() || !group.IsValid()) return Packet();
    const uint8_t status = message.Type() == Midi::MessageType::NoteOn && message.Data(2) == 0
        ? static_cast<uint8_t>(0x80u | (message.Status() & 15u)) : message.Status();
    const uint32_t type = status < 0xF0 ? 2u : 1u;
    return Packet::FromWords((type << 28) | (static_cast<uint32_t>(group.Wire()) << 24) |
        (static_cast<uint32_t>(status) << 16) |
        (static_cast<uint32_t>(message.Data(1)) << 8) | message.Data(2));
}

/** @brief Encodes native MIDI 2.0 channel voice with an explicit UMP group. @ingroup MIDILAR_Ump */
constexpr Packet Encode(Group group, const Midi2::Message& message) noexcept {
    return group.IsValid() && message.IsValid()
        ? Packet::FromWords(0x40000000u | (static_cast<uint32_t>(group.Wire()) << 24) |
                            message.DataWord(0), message.DataWord(1))
        : Packet();
}

/**
 * @brief Decodes one short MIDI 1.0 UMP, discarding its group. @ingroup MIDILAR_Ump
 * Rejects MIDI 2.0, other message types, invalid lengths/data and nonzero
 * reserved fields. Does not implicitly change protocol resolution.
 */
inline Midi::Message DecodeMidi1(const Packet& packet) noexcept {
    Midi::Message result;
    if (!packet.IsValid() || (packet.Type() != MessageType::Midi1ChannelVoice &&
                             packet.Type() != MessageType::System)) return result;
    const uint8_t status = static_cast<uint8_t>(packet.Word(0) >> 16);
    if ((packet.Type() == MessageType::System) != (status >= 0xF0)) return result;
    const uint8_t bytes[3] = {status, static_cast<uint8_t>(packet.Word(0) >> 8),
                            static_cast<uint8_t>(packet.Word(0))};
    // Try exact complete-message lengths; SetRawData validates the status.
    for (size_t size = 1; size <= 3; ++size) {
        if ((size < 3 && bytes[2] != 0) || (size < 2 && bytes[1] != 0)) continue;
        result.SetRawData(bytes, size);
        if (result.IsValid()) return result;
    }
    return result;
}

/** @brief Decodes supported MIDI 2.0 channel voice, discarding the group. @ingroup MIDILAR_Ump */
inline Midi2::Message DecodeMidi2(const Packet& packet) noexcept {
    return packet.IsValid() && packet.Type() == MessageType::Midi2ChannelVoice
        ? Midi2::Message::FromDataWords(packet.Word(0) & 0x00FFFFFFu, packet.Word(1))
        : Midi2::Message();
}
}
#endif
