#ifndef MIDILAR_LOGEXP_LUT_H
#define MIDILAR_LOGEXP_LUT_H

    #include <System/BuildSettings.h>
    #include <math.h>
    #include <AudioFoundation/LUT/BaseLUT.h>

    namespace MIDILAR::AudioFoundation::LUT {

        ///////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @class LogExpLUT
         * @brief A lookup table implementing the log-exp morphing function.
         *
         * This class precomputes values based on:
         * {-1 <= k <= 1}, {c > 0}, 
         * a = 0.5k + 1
         * f(a) = (e^(c * a) - 1) / (e^c - 1)
         * y(x) = M * x^(f(a)) + B
         *
         * @tparam LUT_TYPE The data type stored in the LUT (default: float).
         * @tparam LUT_RANGE_TYPE The range type for indexing the LUT (default: float).
         */
            template<typename LUT_TYPE = float, typename LUT_RANGE_TYPE = float>
            class LogExpLUT : public BaseLUT<LUT_TYPE, LUT_RANGE_TYPE> {
                protected:

                    LUT_TYPE _amplitude;  ///< M: Output amplitude scaling.
                    LUT_TYPE _offset;     ///< B: Output offset.
                    float _morph;      ///< k: Morphing parameter.
                    float _expGain;    ///< c: Exponentiation gain.

                    ///////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Evaluates and populates the LUT with log-exp morphing values.
                     */
                        void Eval() override {
                            size_t bufferSize = this->Size();
                            if (!this->_buffer || bufferSize == 0) return;

                            float a = (0.5f * _morph) + 1.0f;
                            
                            // Compute exp(c) once outside loop
                            float expC = expf(_expGain);
                            float denominator = expC - 1.0f;

                            float f_a;
                            if (fabs(_expGain) < 1e-6f) {
                                // Use first-order approximation when _expGain is near zero
                                f_a = a / (1.0f + _expGain);
                            } else {
                                f_a = (expf(_expGain * a) - 1.0f) / denominator;
                            }

                            for (size_t i = 0; i < bufferSize; i++) {
                                float x = static_cast<float>(i) / (bufferSize - 1); // Normalize x in [0,1]

                                // Compute final y(x) = M * x^(f(0.5k + 1)) + B
                                this->_buffer[i] = static_cast<LUT_TYPE>(_amplitude * powf(x, f_a) + _offset);
                            }
                        }
                    //
                    ///////////////////////////////////////////////////////////////////////////////

                public:

                    ///////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Constructor for LogExpLUT.
                     * @param size The number of samples in the LUT (default: 256).
                     */
                        LogExpLUT(size_t size = 256)
                            : BaseLUT<LUT_TYPE, LUT_RANGE_TYPE>() {
                            _amplitude = 1.0f;
                            _offset = 0.0f;
                            _morph = 0.0f;
                            _expGain = 1.0f; // Must be > 0

                            this->SetInputRange(0.0f, 1.0f);
                            this->_ResizeBuffer(size);
                            Eval();
                        }
                    //
                    ///////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Sets the amplitude (M) and updates the LUT.
                     * @param Amp The new amplitude.
                     */
                        void SetAmplitude(LUT_TYPE Amp) {
                            if (_amplitude == Amp) return;
                            _amplitude = Amp;
                            Eval();
                        }
                    //
                    ///////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Sets the offset (B) and updates the LUT.
                     * @param Offset The new offset.
                     */
                        void SetOffset(LUT_TYPE Offset) {
                            if (_offset == Offset) return;
                            _offset = Offset;
                            Eval();
                        }
                    //
                    ///////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Sets the morph control (k) and updates the LUT.
                     * @param Morph The morphing factor (-1 to 1).
                     */
                        void SetMorph(float Morph) {
                            if (_morph == Morph) return;
                            _morph = Morph;
                            Eval();
                        }
                    //
                    ///////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Sets the exponentiation gain (c) and updates the LUT.
                     * @param ExpGain The exponentiation factor (must be > 0).
                     */
                        void SetExponentiationGain(float ExpGain) {
                            if (_expGain == ExpGain || ExpGain <= 0.0f) return;
                            _expGain = ExpGain;
                            Eval();
                        }
                    //
                    ///////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Gets the amplitude (M).
                     * @return The current amplitude.
                     */
                        LUT_TYPE GetAmplitude() const { return _amplitude; }
                    //
                    ///////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Gets the offset (B).
                     * @return The current offset.
                     */
                        LUT_TYPE GetOffset() const { return _offset; }
                    //
                    ///////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Gets the morphing factor (k).
                     * @return The current morphing parameter.
                     */
                        float GetMorph() const { return _morph; }
                    //
                    ///////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Gets the exponentiation gain (c).
                     * @return The current exponentiation factor.
                     */
                        float GetExponentiationGain() const { return _expGain; }
                    //
                    ///////////////////////////////////////////////////////////////////////////////
            };
        //
        ///////////////////////////////////////////////////////////////////////////////////////////

    }

#endif // MIDILAR_LOGEXP_LUT_H