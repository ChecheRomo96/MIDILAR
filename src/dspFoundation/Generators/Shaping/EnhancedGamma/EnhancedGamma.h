#ifndef MIDILAR_GENERATORS_ENHANCED_GAMMA_H
#define MIDILAR_GENERATORS_ENHANCED_GAMMA_H

    #include <System/BuildSettings.h>
    #include <math.h>

    namespace MIDILAR::dspFoundation::Generators::Shaping {

        
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @brief Generates a 1D gamma-morphed shaping curve.
         *
         * This function fills the provided buffer with a normalized shaping function
         * defined by:
         * \f[
         * y(x) = x^{G^m}
         * \f]
         * where:
         * - \f$ x \in [0, 1] \f$ is linearly spaced across the buffer
         * - \f$ G \f$ is the shaping base (`ExpGain`)
         * - \f$ m \f$ is the morph control (`Morph`), ranging from -1 to 1
         *
         * The output is normalized to the range [0, 1], making it suitable
         * for velocity curves, envelope shaping, or general-purpose nonlinear scaling.s
         *
         * @tparam OUTPUT_TYPE The output data type (e.g., float or uint8_t).
         * @param Buffer Pointer to the output buffer to be filled.
         * @param BufferSize Number of samples in the buffer.
         * @param Morph Morph value in the range [-1, 1] controlling curve shape.
         * @param ExpGain Exponentiation base G, with G ≥ 1 controlling shaping intensity.
         */
            template<typename OUTPUT_TYPE>
            void EnhancedGamma(OUTPUT_TYPE* Buffer, size_t BufferSize, float Morph, float ExpGain, OUTPUT_TYPE OutputGain) {
                if (!Buffer || BufferSize == 0) return;

                float xStep = 1.0f / static_cast<float>(BufferSize - 1);  // ensures last x = 1.0
                float exponent = powf(ExpGain, Morph);
                exponent = fmaxf(exponent, 0.000001f); // avoid undefined powf(0, exponent)
                                
                for (size_t i = 0; i < BufferSize-1; ++i) {
                    float x = i * xStep;
                    float y = powf(x, exponent);  // Normalized shaping
                    Buffer[i] = static_cast<OUTPUT_TYPE>(y * OutputGain);
                }
                
                // Ensure the last sample is exactly OutputGain
                Buffer[BufferSize - 1] = static_cast<OUTPUT_TYPE>(OutputGain);
            }
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////


    }
    
#endif//MIDILAR_GENERATORS_ENHANCED_GAMMA_H
