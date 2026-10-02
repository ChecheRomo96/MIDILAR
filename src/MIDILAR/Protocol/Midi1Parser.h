#ifndef MIDILAR_PROTOCOL_MIDI1_PARSER_H
#define MIDILAR_PROTOCOL_MIDI1_PARSER_H

#include <stdint.h>

#include "Channel.h"
#include "Packet.h"

namespace MIDILAR::Protocol {

/**
 * @brief Translates a MIDI 1.0 byte stream into UMP packets (SPEC-MIDI-2,
 * SPEC-MIDI-8..13).
 * @ingroup MIDILAR_Protocol
 *
 * Feed bytes one at a time with `Parse()`. Channel voice messages become
 * MIDI 1.0 channel voice packets, system messages become system packets and
 * System Exclusive becomes 7-bit System Exclusive packets of up to six bytes,
 * all on the parser's group. The parser never allocates and keeps no
 * payload buffer: a SysEx packet is emitted as soon as the following byte
 * shows whether it is the last one. Use `SysExAssembler` to collect payloads.
 *
 * - Running status is accepted after channel messages; system common and
 *   System Exclusive clear it, real-time bytes do not (SPEC-MIDI-10).
 * - A Note On with velocity 0 is emitted as a Note Off with velocity 0
 *   (SPEC-MIDI-8).
 * - Real-time bytes are emitted at once, anywhere in the stream, and leave
 *   the interrupted message intact (SPEC-MIDI-11).
 * - A status byte where data was expected aborts the incomplete message;
 *   undefined status bytes (`0xF4`, `0xF5`, `0xF9`, `0xFD`) and data bytes
 *   without a status are ignored (SPEC-MIDI-12).
 * - Any status byte other than real-time ends System Exclusive; the payload
 *   so far is emitted as its last packet (SPEC-MIDI-13).
 *
 * With `MIDILAR_PARSER_DIAGNOSTICS` the parser also counts aborted messages
 * and ignored bytes (SPEC-MIDI-15).
 */
class Midi1Parser {
public:
    /** @brief Most packets one byte can produce: a SysEx end and a message. */
    static constexpr uint8_t MaxPackets = 2;

    /** @brief Creates a parser that emits packets on `group` (default: group 1). */
    explicit Midi1Parser(Protocol::Group group = Protocol::Group::FromWire(0)) noexcept
        : _group(group.IsValid() ? group.Wire() : static_cast<uint8_t>(0)) {}

    /**
     * @brief Consumes one byte and writes the completed packets to `out`.
     * @return The number of packets written, `0-2`.
     */
    uint8_t Parse(uint8_t byte, Packet (&out)[MaxPackets]) noexcept {
        if (byte >= 0xF8) {
            if (byte == 0xF9 || byte == 0xFD) {
                CountIgnored();
                return 0;
            }
            out[0] = SystemWord(byte, 0, 0);
            return 1;
        }

        if (byte < 0x80) {
            return ParseData(byte, out);
        }

        if (byte == 0xF4 || byte == 0xF5) {
            CountIgnored();
            return 0;
        }

        uint8_t count = 0;
        if (_inSysEx) {
            out[count++] = SysExPacket(_sysExStarted ? SysExStatus::End : SysExStatus::Complete);
            _inSysEx = false;
            if (byte == 0xF7) {
                return count;
            }
        } else if (byte == 0xF7) {
            CountIgnored();
            return 0;
        }

        if (_status != 0) {
            CountAborted();
        }
        _status = 0;
        _dataCount = 0;

        if (byte == 0xF0) {
            _runningStatus = 0;
            _inSysEx = true;
            _sysExStarted = false;
            _sysExCount = 0;
        } else if (byte >= 0xF0) {
            _runningStatus = 0;
            if (byte == 0xF6) {
                out[count++] = SystemWord(byte, 0, 0);
            } else {
                _status = byte;
                _dataNeeded = byte == 0xF2 ? 2 : 1;
            }
        } else {
            _runningStatus = byte;
            _status = byte;
            _dataNeeded = (byte & 0xF0) == 0xC0 || (byte & 0xF0) == 0xD0 ? 1 : 2;
        }
        return count;
    }

    /** @brief Forgets any partial message, System Exclusive and running status. */
    void Reset() noexcept {
        _runningStatus = 0;
        _status = 0;
        _dataCount = 0;
        _inSysEx = false;
        _sysExStarted = false;
        _sysExCount = 0;
    }

#if defined(MIDILAR_PARSER_DIAGNOSTICS)
    /** @brief Incomplete messages aborted by a status byte. */
    uint32_t AbortedMessages() const noexcept { return _aborted; }

    /** @brief Undefined status bytes, stray data and stray `0xF7` ignored. */
    uint32_t IgnoredBytes() const noexcept { return _ignored; }
#endif

private:
    uint8_t ParseData(uint8_t byte, Packet (&out)[MaxPackets]) noexcept {
        if (_inSysEx) {
            uint8_t count = 0;
            if (_sysExCount == 6) {
                out[count++] = SysExPacket(_sysExStarted ? SysExStatus::Continue : SysExStatus::Start);
                _sysExStarted = true;
            }
            _sysEx[_sysExCount++] = byte;
            return count;
        }

        if (_status == 0) {
            if (_runningStatus == 0) {
                CountIgnored();
                return 0;
            }
            _status = _runningStatus;
            _dataNeeded = (_status & 0xF0) == 0xC0 || (_status & 0xF0) == 0xD0 ? 1 : 2;
        }

        _data[_dataCount++] = byte;
        if (_dataCount < _dataNeeded) {
            return 0;
        }

        const uint8_t status = _status;
        const uint8_t data1 = _data[0];
        const uint8_t data2 = _dataNeeded == 2 ? _data[1] : static_cast<uint8_t>(0);
        _status = 0;
        _dataCount = 0;

        if (status >= 0xF0) {
            out[0] = SystemWord(status, data1, data2);
            return 1;
        }
        const bool noteOnAsOff = (status & 0xF0) == 0x90 && data2 == 0;
        const uint32_t wireStatus = noteOnAsOff ? (0x80u | (status & 0x0Fu)) : status;
        out[0] = Packet::FromWords((static_cast<uint32_t>(0x2) << 28) | (static_cast<uint32_t>(_group) << 24) | (wireStatus << 16) |
                                   (static_cast<uint32_t>(data1) << 8) | data2);
        return 1;
    }

    Packet SystemWord(uint8_t status, uint8_t data1, uint8_t data2) const noexcept {
        return Packet::FromWords((static_cast<uint32_t>(0x1) << 28) | (static_cast<uint32_t>(_group) << 24) |
                                 (static_cast<uint32_t>(status) << 16) | (static_cast<uint32_t>(data1) << 8) |
                                 data2);
    }

    Packet SysExPacket(SysExStatus status) noexcept {
        const uint8_t size = _sysExCount;
        _sysExCount = 0;
        return Packet::SysEx7(Protocol::Group::FromWire(_group), status, _sysEx, size);
    }

    void CountIgnored() noexcept {
#if defined(MIDILAR_PARSER_DIAGNOSTICS)
        ++_ignored;
#endif
    }

    void CountAborted() noexcept {
#if defined(MIDILAR_PARSER_DIAGNOSTICS)
        ++_aborted;
#endif
    }

    uint8_t _group;
    uint8_t _runningStatus = 0;
    uint8_t _status = 0;
    uint8_t _dataNeeded = 0;
    uint8_t _dataCount = 0;
    uint8_t _data[2] = {0, 0};
    bool _inSysEx = false;
    bool _sysExStarted = false;
    uint8_t _sysExCount = 0;
    uint8_t _sysEx[6] = {0, 0, 0, 0, 0, 0};
#if defined(MIDILAR_PARSER_DIAGNOSTICS)
    uint32_t _aborted = 0;
    uint32_t _ignored = 0;
#endif
};

} // namespace MIDILAR::Protocol

#endif // MIDILAR_PROTOCOL_MIDI1_PARSER_H
