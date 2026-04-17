#ifndef MIDILAR_SQUARE_LUT_H
#define MIDILAR_SQUARE_LUT_H

#include <MIDILAR_BuildSettings.h>
#include <math.h>
#include <dspFoundation/LUT/LUT1D/LUT1D.h>
#include <dspFoundation/Generators/Periodic/Periodic.h>

#ifndef TWO_PI
    #define TWO_PI 6.28318530718f
#endif

namespace MIDILAR::dspFoundation::LUT::Periodic {

/**
 * @class SquareLUT
 * @brief Square waveform lookup table generator.
 *
 * This class generates a periodic square wave using the `Eval()` method and stores it
 * in a 1D LUT buffer. It supports optional amplitude scaling, DC offsetting, and phase shifting.
 *
 * Typical use cases include LFOs, gating signals, and binary modulation sources.
 * It inherits from `LUT1D<OUTPUT_TYPE>` and is often flattened for efficient runtime access.
 *
 * @tparam OUTPUT_TYPE The data type used to store waveform samples (e.g., float, int16_t).
 *
 * @see MIDILAR::dspFoundation::LUT::LUT1D
 * @ingroup MIDILAR_dspFoundation_LUT_Periodic
 */
template<typename OUTPUT_TYPE>
class SquareLUT : public LUT::LUT1D<OUTPUT_TYPE> {
private:
    float _phase_offset;  ///< Phase offset in radians.
    float _amplitude;     ///< Output signal amplitude.
    float _offset;        ///< DC offset.

protected:
    /**
     * @brief Populates the LUT buffer with a square waveform.
     */
    void Eval() override {
        MIDILAR::dspFoundation::Generators::Periodic::SquareGenerator<OUTPUT_TYPE>(
            this->GetBuffer(),
            this->BufferSize(),
            _amplitude,
            _offset,
            _phase_offset / TWO_PI
        );
    }

public:
    /**
     * @brief Default constructor.
     * Initializes with 0 phase offset, unit amplitude, and zero DC offset.
     */
    SquareLUT()
        : LUT::LUT1D<OUTPUT_TYPE>(),
          _phase_offset(0.0f),
          _amplitude(1.0f),
          _offset(0.0f)
    {
        Eval();
    }

    /**
     * @brief Resizes the LUT buffer.
     * @param size Number of waveform samples to store.
     * @return True if resizing was successful.
     */
    bool SetBufferSize(size_t size) {
        return LUT::LUT1D<OUTPUT_TYPE>::SetBufferSize(size);
    }

    /**
     * @brief Sets phase offset in normalized [0.0 – 1.0] units.
     */
    void SetPhaseOffset(float Offset, bool Update = true) {
        float newOffset = fmodf(Offset * TWO_PI, TWO_PI);
        if (newOffset < 0.0f) newOffset += TWO_PI;
        if (_phase_offset == newOffset) return;
        _phase_offset = newOffset;
        if (Update) Eval();
    }

    /**
     * @brief Sets phase offset in radians.
     */
    void SetPhaseOffsetRads(float Offset, bool Update = true) {
        float newOffset = fmodf(Offset, TWO_PI);
        if (newOffset < 0.0f) newOffset += TWO_PI;
        if (_phase_offset == newOffset) return;
        _phase_offset = newOffset;
        if (Update) Eval();
    }

    /**
     * @brief Sets phase offset in degrees.
     */
    void SetPhaseOffsetDeg(float Offset, bool Update = true) {
        float newOffset = fmodf(Offset * (TWO_PI / 360.0f), TWO_PI);
        if (newOffset < 0.0f) newOffset += TWO_PI;
        if (_phase_offset == newOffset) return;
        _phase_offset = newOffset;
        if (Update) Eval();
    }

    /**
     * @brief Sets waveform amplitude.
     */
    void SetAmplitude(float Amp, bool Update = true) {
        if (_amplitude == Amp) return;
        _amplitude = Amp;
        if (Update) Eval();
    }

    /**
     * @brief Sets vertical DC offset.
     */
    void SetDCOffset(float Offset, bool Update = true) {
        if (_offset == Offset) return;
        _offset = Offset;
        if (Update) Eval();
    }
};

} // namespace MIDILAR::dspFoundation::LUT::Periodic

#endif // MIDILAR_SQUARE_LUT_H
