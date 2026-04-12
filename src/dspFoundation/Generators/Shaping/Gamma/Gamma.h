#ifndef MIDILAR_GENERATORS_GAMMA_H
#define MIDILAR_GENERATORS_GAMMA_H

    #include <System/BuildSettings.h>
    #include <math.h>

    namespace MIDILAR::dspFoundation::Generators::Shaping {

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @brief Generates a 1D fixed-gamma shaping curve.
         *
         * Fills the given buffer with a static power curve:
         * \f[
         * y(x) = x^\gamma
         * \f]
         * where:
         * - \f$ x \in [0, 1] \f$ is linearly spaced
         * - \f$ \gamma > 0 \f$ is a fixed exponent
         *
         * @tparam OUTPUT_TYPE The output type (e.g., float, uint8_t).
         * @param Buffer Output buffer to write the curve into.
         * @param BufferSize Number of samples.
         * @param Gamma The gamma exponent to apply.
         * @param OutputGain Scaling factor applied to the output values (must match OUTPUT_TYPE's intent).
         */
        template<typename OUTPUT_TYPE>
        void Gamma(OUTPUT_TYPE* Buffer, size_t BufferSize, float Gamma, OUTPUT_TYPE OutputGain) {
            if (!Buffer || BufferSize == 0) return;

            Gamma = fmaxf(Gamma, 0.000001f);  // Avoid domain errors

            float xStep = 1.0f / static_cast<float>(BufferSize - 1);

            for (size_t i = 0; i < BufferSize - 1; ++i) {
                float x = i * xStep;
                float y = powf(x, Gamma);
                Buffer[i] = static_cast<OUTPUT_TYPE>(y * static_cast<float>(OutputGain));
            }

            Buffer[BufferSize - 1] = OutputGain;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////

    }

#endif // MIDILAR_GENERATORS_GAMMA_H
