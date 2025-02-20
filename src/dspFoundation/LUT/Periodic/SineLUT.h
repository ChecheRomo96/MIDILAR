#ifndef MIDILAR_SINE_LUT_H
#define MIDILAR_SINE_LUT_H

    #include <System/BuildSettings.h>
    #include <math.h>
    #include <dspFoundation/LUT/LUT1D/LUT1D.h>

    #ifndef TWO_PI
        #define TWO_PI 6.28318530718f
    #endif

    namespace MIDILAR::dspFoundation::LUT::Periodic {

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @class SineLUT
         * @brief A sawtooth wave lookup table.
         * 
         * This class precomputes sawtooth wave values for fast lookup and interpolation.
         * 
         * @tparam OUTPUT_TYPE The range type for indexing the LUT (default: float).
         */
            template<typename OUTPUT_TYPE>
            class SineLUT : public LUT::LUT1D<OUTPUT_TYPE> {
            private:
                float _phase_offset;  // Stored in Radians
                float _amplitude;
                float _offset;

            protected:
                /**
                 * @brief Evaluates and populates the LUT with sawtooth wave values.
                 */
                void Eval() override {
                    size_t bufferSize = this->BufferSize();

                    if (bufferSize == 0) return;
                    
                    float phaseStep = TWO_PI / static_cast<float>(bufferSize);
                    float phaseOffsetScaled = _phase_offset * TWO_PI;  // Precompute once

                    for (size_t i = 0; i < bufferSize; i++) {
                        float phase = (i * phaseStep) + phaseOffsetScaled;
                        this->SetData(i, static_cast<OUTPUT_TYPE>(_amplitude * sinf(phase) + _offset));
                    }
                }

            public:
                /**
                 * @brief Constructor for SineLUT.
                 */
                SineLUT()
                    : LUT1D<OUTPUT_TYPE>(),
                    _phase_offset(0.0f),
                    _amplitude(1.0f),
                    _offset(0.0f) {
                    Eval();
                }
                
                bool SetBufferSize(size_t size){
                    return LUT::LUT1D<OUTPUT_TYPE>::SetBufferSize(size);
                }

                void SetPhaseOffset(float Offset, bool Update = true) {
                    float newOffset = fmodf(Offset * TWO_PI, TWO_PI);
                    if (newOffset < 0.0f) newOffset += TWO_PI;
                    if (_phase_offset == newOffset) return;
                    _phase_offset = newOffset;
                    if (Update) Eval();
                }

                void SetPhaseOffsetRads(float Offset, bool Update = true) {
                    float newOffset = fmodf(Offset, TWO_PI);
                    if (newOffset < 0.0f) newOffset += TWO_PI;
                    if (_phase_offset == newOffset) return;
                    _phase_offset = newOffset;
                    if (Update) Eval();
                }

                void SetPhaseOffsetDeg(float Offset, bool Update = true) {
                    float newOffset = fmodf(Offset * (TWO_PI / 360.0f), TWO_PI);
                    if (newOffset < 0.0f) newOffset += TWO_PI;
                    if (_phase_offset == newOffset) return;
                    _phase_offset = newOffset;
                    if (Update) Eval();
                }

                void SetAmplitude(float Amp, bool Update = true) {
                    if (_amplitude == Amp) return;
                    _amplitude = Amp;
                    if (Update) Eval();
                }

                void SetDCOffset(float Offset, bool Update = true) {
                    if (_offset == Offset) return;
                    _offset = Offset;
                    if (Update) Eval();
                }
            };

    } // namespace MIDILAR::dspFoundation::LUT::Periodic

#endif//MIDILAR_SINE_LUT_H
