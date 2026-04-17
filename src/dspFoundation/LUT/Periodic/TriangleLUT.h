#ifndef MIDILAR_TRIANGLE_LUT_H
#define MIDILAR_TRIANGLE_LUT_H

#include <MIDILAR_BuildSettings.h>
#include <math.h>
#include <dspFoundation/LUT/LUT1D/LUT1D.h>
#include <dspFoundation/Generators/Periodic/Periodic.h>

#ifndef TWO_PI
    #define TWO_PI 6.28318530718f
#endif

namespace MIDILAR::dspFoundation::LUT::Periodic {

/**
 * @class TriangleLUT
 * @brief Triangle waveform lookup table generator.
 *
 * `TriangleLUT` fills a 1D LUT with values representing a triangle wave.
 * This waveform is useful in both audio-rate and control-rate scenarios.
 * 
 * The class allows configuration of:
 * - Phase offset in radians/degrees/normalized units
 * - Amplitude scaling
 * - DC offset (vertical shift)
 *
 * After configuration, the waveform can be flattened into a `LUT1D` base container
 * for efficient playback.
 *
 * @tparam OUTPUT_TYPE The type used for LUT sample values (e.g., float, int16_t).
 *
 * @see MIDILAR::dspFoundation::LUT::LUT1D
 * @ingroup MIDILAR_dspFoundation_LUT_Periodic
 */
template<typename OUTPUT_TYPE>
class TriangleLUT : public LUT::LUT1D<OUTPUT_TYPE> {
private:
    float _phase_offset;  ///< Phase offset in radians.
    float _amplitude;     ///< Output amplitude.
    float _offset;        ///< Vertical DC offset.

protected:
    /**
     * @brief Populates the LUT with triangle waveform values.
     */
    void Eval() override {
        MIDILAR::dspFoundation::Generators::Periodic::Triangle<OUTPUT_TYPE>(
            this->GetBuffer(),
            this->BufferSize(),
            _amplitude,
            _offset,
            _phase_offset / TWO_PI
        );
    }

public:
    /**
     * @brief Constructs a triangle LUT with default parameters.
     */
    TriangleLUT()
        : LUT::LUT1D<OUTPUT_TYPE>(),
          _phase_offset(0.0f),
          _amplitude(1.0f),
          _offset(0.0f)
    {
        Eval();
    }

    /**
     * @brief Sets the LUT buffer size and triggers evaluation.
     * @param size Number of LUT bins.
     * @return True if resized successfully.
     */
    bool SetBufferSize(size_t size) {
        return LUT::LUT1D<OUTPUT_TYPE>::SetBufferSize(size);
    }

    /**
     * @brief Sets phase offset in normalized [0.0–1.0] units.
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
     * @brief Sets DC offset (vertical shift).
     */
    void SetDCOffset(float Offset, bool Update = true) {
        if (_offset == Offset) return;
        _offset = Offset;
        if (Update) Eval();
    }
};

} // namespace MIDILAR::dspFoundation::LUT::Periodic

#endif // MIDILAR_TRIANGLE_LUT_H
