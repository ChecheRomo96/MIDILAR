#ifndef MIDILAR_SINE_LUT_H
#define MIDILAR_SINE_LUT_H

#include <System/BuildSettings.h>
#include <math.h>
#include <dspFoundation/LUT/LUT1D/LUT1D.h>
#include <dspFoundation/Generators/Periodic/Periodic.h>

#ifndef TWO_PI
    #define TWO_PI 6.28318530718f
#endif

namespace MIDILAR::dspFoundation::LUT::Periodic {

//! @class SineLUT
/**
 * @brief Sine waveform lookup table generator.
 *
 * This class precomputes sine wave values into a 1D lookup table using the `Eval()` method.
 * The buffer can be configured by amplitude, DC offset, and phase offset (in radians, degrees, or normalized).
 *
 * It inherits from `LUT1D<OUTPUT_TYPE>` and is typically used to populate a base LUT container
 * for flattened, real-time waveform access.
 *
 * @tparam OUTPUT_TYPE The type used to store the waveform values (e.g., float, int16_t).
 *
 * @see MIDILAR::dspFoundation::LUT::LUT1D
 * @ingroup MIDILAR_dspFoundation_LUT_Periodic
 */
template<typename OUTPUT_TYPE>
class SineLUT : public LUT::LUT1D<OUTPUT_TYPE> {
private:
    float _phase_offset;  ///< Phase offset in radians.
    float _amplitude;     ///< Peak amplitude of the sine wave.
    float _offset;        ///< DC offset (vertical shift).

protected:
    /**
     * @brief Evaluates and populates the LUT with sine wave values.
     */
    void Eval() override {
        MIDILAR::dspFoundation::Generators::Periodic::Sine<OUTPUT_TYPE>(
            this->GetBuffer(),
            this->BufferSize(),
            _amplitude,
            _offset,
            _phase_offset / TWO_PI
        );
    }

public:
    /**
     * @brief Constructor.
     *
     * Initializes the sine LUT with default parameters (amplitude = 1, offset = 0, phase = 0).
     */
    SineLUT()
        : LUT1D<OUTPUT_TYPE>(),
          _phase_offset(0.0f),
          _amplitude(1.0f),
          _offset(0.0f)
    {
        Eval();
    }

    /**
     * @brief Resizes the LUT buffer.
     * @param size Number of samples in the LUT.
     * @return True if buffer was resized successfully.
     */
    bool SetBufferSize(size_t size) {
        return LUT::LUT1D<OUTPUT_TYPE>::SetBufferSize(size);
    }

    /**
     * @brief Sets the phase offset in normalized units [0.0–1.0].
     * @param Offset Normalized offset (1.0 = 2π radians).
     * @param Update If true, triggers LUT recalculation.
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
     * @param Offset Offset value in radians.
     * @param Update If true, triggers LUT recalculation.
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
     * @param Offset Offset value in degrees.
     * @param Update If true, triggers LUT recalculation.
     */
    void SetPhaseOffsetDeg(float Offset, bool Update = true) {
        float newOffset = fmodf(Offset * (TWO_PI / 360.0f), TWO_PI);
        if (newOffset < 0.0f) newOffset += TWO_PI;
        if (_phase_offset == newOffset) return;
        _phase_offset = newOffset;
        if (Update) Eval();
    }

    /**
     * @brief Sets the sine wave amplitude.
     * @param Amp Amplitude value.
     * @param Update If true, triggers LUT recalculation.
     */
    void SetAmplitude(float Amp, bool Update = true) {
        if (_amplitude == Amp) return;
        _amplitude = Amp;
        if (Update) Eval();
    }

    /**
     * @brief Sets the DC offset of the waveform.
     * @param Offset Value to add to the waveform vertically.
     * @param Update If true, triggers LUT recalculation.
     */
    void SetDCOffset(float Offset, bool Update = true) {
        if (_offset == Offset) return;
        _offset = Offset;
        if (Update) Eval();
    }
};

} // namespace MIDILAR::dspFoundation::LUT::Periodic

#endif // MIDILAR_SINE_LUT_H
