#ifndef MIDILAR_EXAMPLES_DEVICES_TRANSPOSE_THRU_SHARED_H
#define MIDILAR_EXAMPLES_DEVICES_TRANSPOSE_THRU_SHARED_H

#include <stdint.h>

#include <MIDILAR/Devices/Filters.h>
#include <MIDILAR/Devices/Transforms.h>
#include <MIDILAR/Protocol/Midi1Encoder.h>
#include <MIDILAR/Protocol/Midi1Parser.h>

namespace MIDILARExamples::Devices::TransposeThru {

/**
 * A MIDI 1.0 thru box: bytes in, parsed to UMP, transposed, kept on
 * C major and encoded back to bytes.
 */
class Thru {
public:
    /** Most bytes one input byte can produce. */
    static constexpr uint8_t Capacity =
        MIDILAR::Protocol::Midi1Parser::MaxPackets * MIDILAR::Protocol::Midi1Encoder::MaxBytes;

    Thru() noexcept;

    /** Sets the transposition in semitones. */
    void SetSemitones(int32_t semitones) noexcept;

    /** Consumes one input byte and returns how many bytes were written to `out`. */
    uint8_t Process(uint8_t byte, uint8_t (&out)[Capacity]);

private:
    void Collect(const MIDILAR::Protocol::Packet& packet);

    MIDILAR::Protocol::Midi1Parser _parser;
    MIDILAR::Devices::Transpose _transpose;
    MIDILAR::Devices::ScaleFilter _scale;
    MIDILAR::Protocol::Midi1Encoder _encoder;
    uint8_t* _out = nullptr;
    uint8_t _count = 0;
};

} // namespace MIDILARExamples::Devices::TransposeThru

#endif // MIDILAR_EXAMPLES_DEVICES_TRANSPOSE_THRU_SHARED_H
