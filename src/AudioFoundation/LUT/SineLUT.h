#ifndef MIDILAR_SINE_LUT_H
#define MIDILAR_SINE_LUT_H

#include <MIDILAR_BuildSettings.h>
#include <math.h>
#include <AudioFoundation/LUT/BaseLUT.h>

namespace MIDILAR::AudioFoundation::LUT {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @class SineLUT
     * @brief A sine wave lookup table.
     * 
     * This class precomputes sine wave values for fast lookup and interpolation.
     * 
     * @tparam LUT_TYPE The data type stored in the LUT (default: float).
     * @tparam LUT_RANGE_TYPE The range type for indexing the LUT (default: float).
     */
        template<typename LUT_TYPE = float, typename LUT_RANGE_TYPE = float>
        class SineLUT : public BaseLUT<LUT_TYPE, LUT_RANGE_TYPE> {
            protected:

                ///////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Evaluates and populates the LUT with sine wave values.
                 */
                    void Eval() override {
                        size_t bufferSize = this->Size();
                        if (!this->_buffer || bufferSize == 0) return;

                        float phaseStep = 2.0f * M_PI / (bufferSize);
                        for (size_t i = 0; i < bufferSize; i++) {
                            float phase = (i * phaseStep) + (_phase_offset * 2.0f * M_PI);
                            this->_buffer[i] = static_cast<LUT_TYPE>(_amplitude * sin(phase) + _offset);
                        }
                    }
                //
                ///////////////////////////////////////////////////////////////////////////////////
                //

                    float _phase_offset;
                    float _amplitude;
                    float _offset;
                //
                ///////////////////////////////////////////////////////////////////////////////////

            public:

                ///////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Constructor for SineLUT.
                 * @param size The number of samples in the LUT (default: 256).
                 */
                    SineLUT(size_t size = 256)
                        : BaseLUT<LUT_TYPE, LUT_RANGE_TYPE>() // Ensure correct BaseLUT initialization
                    {
                        _phase_offset = 0.0f;
                        _amplitude = 1.0f;
                        _offset = 0.0f;

                        this->SetInputRange(0.0f, 1.0f); // Ensure proper lookup indexing
                        this->SetBufferSize(size);
                        Eval();
                    }
                //
                ///////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Sets the phase offset and updates the LUT.
                 * @param Offset The phase offset value in wavelengths.
                 */
                    void SetPhaseOffset(float Offset) {
                        float newOffset = Offset - static_cast<int>(Offset); // Properly handle full cycles
                        if (newOffset < 0.0f) newOffset += 1.0f; // Ensure positive wrap-around
                        if (_phase_offset == newOffset) return; // Avoid unnecessary recalculations

                        _phase_offset = newOffset;
                        Eval();
                    }
                //
                ///////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Sets the amplitude and updates the LUT.
                 * @param Amp The new amplitude.
                 */
                    void SetAmplitude(float Amp) {
                        if (_amplitude == Amp) return; // Avoid redundant evaluations

                        _amplitude = Amp;
                        Eval();
                    }
                //
                ///////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Sets the DC offset and updates the LUT.
                 * @param Offset The new DC offset.
                 */
                    void SetDCOffset(float Offset) {
                        if (_offset == Offset) return; // Avoid redundant evaluations

                        _offset = Offset;
                        Eval();
                    }
                //
                ///////////////////////////////////////////////////////////////////////////////////
        };
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////

}

#endif // MIDILAR_SINE_LUT_H
