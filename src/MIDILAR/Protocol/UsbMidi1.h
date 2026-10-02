#ifndef MIDILAR_PROTOCOL_USB_MIDI1_H
#define MIDILAR_PROTOCOL_USB_MIDI1_H

#include <stdint.h>

#include "Channel.h"
#include "Midi1Encoder.h"
#include "Packet.h"

namespace MIDILAR::Protocol {

/**
 * @brief Translates one USB MIDI 1.0 event packet into a UMP packet
 * (SPEC-USB-1).
 * @ingroup MIDILAR_Protocol
 *
 * An event is four bytes: the cable number (high nibble) and Code Index
 * Number (low nibble), then up to three MIDI bytes. The cable becomes the
 * UMP group. System Exclusive events become 7-bit System Exclusive packets
 * of up to three bytes, marked Start, Continue, End or Complete from the
 * `0xF0` and `0xF7` they carry, so no state is needed. A Note On with
 * velocity 0 becomes a Note Off (SPEC-MIDI-8).
 *
 * @return `true` and the packet in `out`, or `false` for reserved,
 * malformed or empty events.
 */
inline bool DecodeUsbMidi1(const uint8_t (&event)[4], Packet& out) noexcept {
    const Group group = Group::FromWire(event[0] >> 4);
    const uint8_t cin = static_cast<uint8_t>(event[0] & 0x0Fu);
    const uint8_t b1 = event[1];
    const uint8_t b2 = event[2];
    const uint8_t b3 = event[3];

    if (cin == 0xF) {
        // Single byte: real-time messages.
        if (b1 < 0xF8 || b1 == 0xF9 || b1 == 0xFD) {
            return false;
        }
        out = Packet::System(group, static_cast<SystemStatus>(b1));
        return true;
    }

    if (cin >= 0x8) {
        // Channel voice: the CIN repeats the status nibble.
        if ((b1 >> 4) != cin || b2 > 127 || b3 > 127) {
            return false;
        }
        const bool oneData = cin == 0xC || cin == 0xD;
        const bool noteOnAsOff = cin == 0x9 && b3 == 0;
        const uint32_t status = noteOnAsOff ? (0x80u | (b1 & 0x0Fu)) : b1;
        out = Packet::FromWords((static_cast<uint32_t>(0x2) << 28) | (static_cast<uint32_t>(group.Wire()) << 24) |
                                (status << 16) | (static_cast<uint32_t>(b2) << 8) | (oneData ? 0u : b3));
        return true;
    }

    if (cin == 0x2 || cin == 0x3) {
        // Two- and three-byte system common.
        if (cin == 0x2 && b1 == 0xF1) {
            out = Packet::TimeCode(group, b2);
        } else if (cin == 0x2 && b1 == 0xF3) {
            out = Packet::SongSelect(group, b2);
        } else if (cin == 0x3 && b1 == 0xF2 && b2 <= 127 && b3 <= 127) {
            out = Packet::SongPosition(group, static_cast<uint16_t>((b3 << 7) | b2));
        } else {
            return false;
        }
        return out.IsValid();
    }

    if (cin == 0x5 && b1 == 0xF6) {
        out = Packet::System(group, SystemStatus::TuneRequest);
        return true;
    }

    if (cin >= 0x4 && cin <= 0x7) {
        // System Exclusive: 4 = start or continue (3 bytes), 5/6/7 = ends
        // with 1/2/3 bytes.
        const uint8_t size = cin == 0x4 ? 3 : static_cast<uint8_t>(cin - 0x4);
        const uint8_t bytes[3] = {b1, b2, b3};
        const bool start = bytes[0] == 0xF0;
        const bool end = cin != 0x4 && bytes[size - 1] == 0xF7;
        if (cin != 0x4 && !end) {
            return false;
        }
        const uint8_t first = start ? 1 : 0;
        const uint8_t count = static_cast<uint8_t>(size - first - (end ? 1 : 0));
        const SysExStatus status = start ? (end ? SysExStatus::Complete : SysExStatus::Start)
                                         : (end ? SysExStatus::End : SysExStatus::Continue);
        out = Packet::SysEx7(group, status, bytes + first, count);
        return out.IsValid();
    }

    return false;
}

/**
 * @brief Translates UMP packets into USB MIDI 1.0 event packets
 * (SPEC-USB-2).
 * @ingroup MIDILAR_Protocol
 *
 * Packets are first turned into MIDI 1.0 bytes by a `Midi1Encoder` (so
 * MIDI 2.0 channel voice is scaled down), then packed into events on the
 * cable equal to the packet's group. System Exclusive is split into
 * three-byte events; bytes that do not fill an event wait, per cable, for
 * the next packet of the same message. Packets without a MIDI 1.0 form
 * produce no events.
 */
class UsbMidi1Encoder {
public:
    /** @brief Most events one packet can produce. */
    static constexpr uint8_t MaxEvents = 4;

    /** @brief One USB MIDI 1.0 event packet. */
    using Event = uint8_t[4];

    /**
     * @brief Writes the events of `packet` to `out`.
     * @return The number of events written, `0-4`.
     */
    uint8_t Encode(const Packet& packet, Event (&out)[MaxEvents]) noexcept {
        const Group group = packet.Group();
        if (!group.IsValid()) {
            return 0;
        }
        uint8_t bytes[Midi1Encoder::MaxBytes];
        const uint8_t size = _encoder.Encode(packet, bytes);
        Cable& cable = _cables[group.Wire()];
        const uint8_t prefix = static_cast<uint8_t>(group.Wire() << 4);
        uint8_t count = 0;
        uint8_t index = 0;
        while (index < size) {
            const uint8_t byte = bytes[index];
            if (byte >= 0xF8) {
                Put(out[count++], static_cast<uint8_t>(prefix | 0xF), byte, 0, 0);
                ++index;
            } else if (byte == 0xF0 || cable.inSysEx) {
                if (byte == 0xF0) {
                    cable.inSysEx = true;
                    cable.count = 0;
                }
                cable.pending[cable.count++] = byte;
                if (byte == 0xF7) {
                    Put(out[count++], static_cast<uint8_t>(prefix | (0x4 + cable.count)), cable.pending[0],
                        cable.count > 1 ? cable.pending[1] : 0, cable.count > 2 ? cable.pending[2] : 0);
                    cable.inSysEx = false;
                    cable.count = 0;
                } else if (cable.count == 3) {
                    Put(out[count++], static_cast<uint8_t>(prefix | 0x4), cable.pending[0], cable.pending[1],
                        cable.pending[2]);
                    cable.count = 0;
                }
                ++index;
            } else if (byte < 0x80) {
                // System Exclusive data without a start on this cable.
                ++index;
            } else {
                const uint8_t length = MessageLength(byte);
                const uint8_t cin = byte < 0xF0 ? static_cast<uint8_t>(byte >> 4)
                    : byte == 0xF2               ? 0x3
                    : byte == 0xF6               ? 0x5
                                                 : 0x2;
                Put(out[count++], static_cast<uint8_t>(prefix | cin), byte,
                    length > 1 ? bytes[index + 1] : 0, length > 2 ? bytes[index + 2] : 0);
                index = static_cast<uint8_t>(index + length);
            }
        }
        return count;
    }

private:
    struct Cable {
        uint8_t pending[3];
        uint8_t count;
        bool inSysEx;
    };

    static uint8_t MessageLength(uint8_t status) noexcept {
        return (status & 0xF0) == 0xC0 || (status & 0xF0) == 0xD0 || status == 0xF1 || status == 0xF3 ? 2
            : status == 0xF6                                                                          ? 1
                                                                                                      : 3;
    }

    static void Put(Event& event, uint8_t header, uint8_t b1, uint8_t b2, uint8_t b3) noexcept {
        event[0] = header;
        event[1] = b1;
        event[2] = b2;
        event[3] = b3;
    }

    Midi1Encoder _encoder;
    Cable _cables[16] = {};
};

} // namespace MIDILAR::Protocol

#endif // MIDILAR_PROTOCOL_USB_MIDI1_H
