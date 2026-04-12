#ifndef MIDILAR_SAW_LUT_H
#define MIDILAR_SAW_LUT_H

#include <System/BuildSettings.h>
#include <math.h>
#include <dspFoundation/LUT/LUT1D/LUT1D.h>
#include <dspFoundation/Generators/Periodic/Periodic.h>

#ifndef TWO_PI
    #define TWO_PI 6.28318530718f
#endif

namespace MIDILAR::dspFoundation::LUT::Periodic {

/**
 * @class SawLUT
 * @brief Sawtooth waveform lookup table generator.
 *
 * `SawLUT` precomputes a unipolar or bipolar sawtooth waveform into a LUT buffer
 * using the `Eval()` method. This LUT can be used for real-time audio or modulation purposes.
 *
 * Parameters such as amplitude, offset, and phase can be adjusted prior to regeneration.
 * Typically, instances of this class are flattened into a `LUT1D` container for lightweight runtime use.
 *
 * @tparam OUTPUT_TYPE The type of the waveform buffer (e.g., float, int16_t).
 *
 * @see MIDILAR::dspFoundation::LUT::LUT1D
 * @ingroup MIDILAR_dspFoundation_LUT_Periodic
 */
template<typename OUTPUT_TYPE>
class SawLUT : public LUT::LUT1D<OUTPUT_TYPE> {
private:
    float _phase_offset;  ///< Phase offset in radians.
    float _amplitude;     ///< Wave amplitude (peak-to-peak).
    float _offset;        ///< DC offset (vertical shift).

protected:
    /**
     * @brief Populates the buffer with a sawtooth waveform.
     */
    void Eval() override {
        MIDILAR::dspFoundation::Generators::Periodic::SawtoothGenerator<OUTPUT_TYPE>(
            this->GetBuffer(),
            this->BufferSize(),
            _amplitude,
            _offset,
            _phase_offset / TWO_PI
        );
    }

public:
    /**
     * @brief Constructs a sawtooth waveform with default parameters.
     */
    SawLUT()
        : LUT::LUT1D<OUTPUT_TYPE>(),
          _phase_offset(0.0f),
          _amplitude(1.0f),
          _offset(0.0f)
    {
        Eval();
    }

    /**
     * @brief Resizes the LUT buffer and re-evaluates the waveform.
     * @param size Number of samples in the LUT.
     * @return True if resizing and regeneration were successful.
     */
    bool SetBufferSize(size_t size) {
        return LUT::LUT1D<OUTPUT_TYPE>::SetBufferSize(size);
    }

    /**
     * @brief Sets the phase offset in normalized units [0.0–1.0].
     */
    void SetPhaseOffset(float Offset, bool Update = true) {
        float newOffset = fmodf(Offset * TWO_PI, TWO_PI);
        if (newOffset < 0.0f) newOffset += TWO_PI;
        if (_phase_offset == newOffset) return;
        _phase_offset = newOffset;
        if (Update) Eval();
    }

    /**
     * @brief Sets the phase offset in radians.
     */
    void SetPhaseOffsetRads(float Offset, bool Update = true) {
        float newOffset = fmodf(Offset, TWO_PI);
        if (newOffset < 0.0f) newOffset += TWO_PI;
        if (_phase_offset == newOffset) return;
        _phase_offset = newOffset;
        if (Update) Eval();
    }

    /**
     * @brief Sets the phase offset in degrees.
     */
    void SetPhaseOffsetDeg(float Offset, bool Update = true) {
        float newOffset = fmodf(Offset * (TWO_PI / 360.0f), TWO_PI);
        if (newOffset < 0.0f) newOffset += TWO_PI;
        if (_phase_offset == newOffset) return;
        _phase_offset = newOffset;
        if (Update) Eval();
    }

    /**
     * @brief Sets the waveform amplitude.
     */
    void SetAmplitude(float Amp, bool Update = true) {
        if (_amplitude == Amp) return;
        _amplitude = Amp;
        if (Update) Eval();
    }

    /**
     * @brief Sets the DC offset of the waveform.
     */
    void SetDCOffset(float Offset, bool Update = true) {
        if (_offset == Offset) return;
        _offset = Offset;
        if (Update) Eval();
    }
};

} // namespace MIDILAR::dspFoundation::LUT::Periodic

#endif // MIDILAR_SAW_LUT_H
