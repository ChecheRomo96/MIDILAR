#ifndef MIDILAR_GENERATORS_GAMMA_H
#define MIDILAR_GENERATORS_GAMMA_H

    #include <MIDILAR_BuildSettings.h>
    #include <math.h>

    namespace MIDILAR::DspCore::Generators::Shaping {

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @brief Generates a 1D fixed-gamma shaping curve.
         *
         * Fills the given buffer with a static power curve:
         * \f[
         * y(x) = x^\gamma
         * \f]
         * where:
         * \f[ x \in [0, 1], \quad \gamma \in (0, \infty) \f] 
         * - \f$ x \f$ is linearly spaced across the buffer
         * - \f$ \gamma \f$ is the gamma exponent controlling the curve's shape
         *
         * @tparam T The output type (e.g., float, uint8_t).
         * @param Buffer Output buffer to write the curve into.
         * @param BufferSize Number of samples.
         * @param GammaValue The gamma exponent to apply.
         * @param OutputScale Output scaling factor. Use 1.0f for normalized floating-point output, or values such as 255 for integer LUTs.
         */
        template<typename T>
        void Gamma(
            T* Buffer,
            size_t BufferSize,
            float GammaValue,
            T OutputScale = static_cast<T>(1.0f)
        ) {
            if (!Buffer || BufferSize == 0) return;

            if (BufferSize == 1) {
                Buffer[0] = OutputScale;
                return;
            }

            GammaValue = fmaxf(GammaValue, 0.000001f);

            const float xStep = 1.0f / static_cast<float>(BufferSize - 1);

            for (size_t i = 0; i < BufferSize - 1; ++i) {
                const float x = static_cast<float>(i) * xStep;
                const float y = powf(x, GammaValue);

                Buffer[i] = static_cast<T>(
                    y * static_cast<float>(OutputScale)
                );
            }

            Buffer[BufferSize - 1] = OutputScale;
        }
    }

    #if defined(MIDILAR_DSP_LUT1D)

        #include <DspCore/LUT/LUT1D.h>

        namespace MIDILAR::DspCore::Generators::Shaping {

            template<typename T>
            bool GammaLUT(
                MIDILAR::DspCore::LUT::LUT1D<T>& lut,
                float GammaValue,
                T OutputScale
            ) {
                if (!lut.GetBuffer() || lut.Size() == 0) {
                    return false;
                }

                Gamma<T>(
                    lut.GetBuffer(),
                    lut.Size(),
                    GammaValue,
                    OutputScale
                );

                return true;
            }

        } // namespace MIDILAR::DspCore::Generators::Shaping

    #endif // MIDILAR_DSP_LUT1D

    #if defined(MIDILAR_DSP_LUT2D)

        #include <DspCore/LUT/LUT2D.h>

        namespace MIDILAR::DspCore::Generators::Shaping {

            template<typename T>
            bool GammaLUT2D(
                MIDILAR::DspCore::LUT::LUT2D<T>& lut,
                float GammaMin,
                float GammaMax,
                T OutputScale = static_cast<T>(1.0f)
            ) {
                if (!lut.GetBuffer() || lut.Width() == 0 || lut.Height() == 0) {
                    return false;
                }

                const size_t width  = lut.Width();
                const size_t height = lut.Height();

                const float gammaStep = (height > 1)
                    ? (GammaMax - GammaMin) / static_cast<float>(height - 1)
                    : 0.0f;

                for (size_t y = 0; y < height; ++y) {
                    const float gammaValue = (height > 1)
                        ? GammaMin + static_cast<float>(y) * gammaStep
                        : GammaMin;

                    T* rowBuffer = lut.GetBuffer() + y * width;

                    Gamma(
                        rowBuffer,
                        width,
                        gammaValue,
                        OutputScale
                    );
                }

                return true;
            }

        } // namespace MIDILAR::DspCore::Generators::Shaping

    #endif // MIDILAR_DSP_LUT2D

#endif // MIDILAR_GENERATORS_GAMMA_H
