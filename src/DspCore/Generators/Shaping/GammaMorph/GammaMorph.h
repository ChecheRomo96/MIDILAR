#ifndef MIDILAR_GENERATORS_SHAPING_GAMMA_MORPH_H
#define MIDILAR_GENERATORS_SHAPING_GAMMA_MORPH_H

    #include <MIDILAR_BuildSettings.h>
    #include <stddef.h>
    #include <math.h>

    namespace MIDILAR::DspCore::Generators::Shaping {

        template<typename T>
        void GammaMorph(
            T* Buffer,
            size_t BufferSize,
            float Morph,
            float Gain,
            T Amplitude
        ) {
            if (!Buffer || BufferSize == 0) return;

            if (BufferSize == 1) {
                Buffer[0] = static_cast<T>(Amplitude);
                return;
            }

            if (Gain < 0.000001f) {
                Gain = 0.000001f;
            }

            float exponent = powf(Gain, Morph);

            const float xStep = 1.0f / static_cast<float>(BufferSize - 1);

            for (size_t i = 0; i < BufferSize - 1; ++i) {
                const float x = static_cast<float>(i) * xStep;
                const float y = powf(x, exponent);
                Buffer[i] = static_cast<T>(y * Amplitude);
            }

            Buffer[BufferSize - 1] = static_cast<T>(Amplitude);
        }

    } // namespace MIDILAR::DspCore::Generators::Shaping

    #if defined(MIDILAR_DSP_LUT1D)

        #include <DspCore/LUT/LUT1D/LUT1D.h>

        namespace MIDILAR::DspCore::Generators::Shaping {

        template<typename T>
        bool GammaMorphLUT1D(
            MIDILAR::DspCore::LUT::LUT1D<T>& lut,
            float Morph,
            float Gain,
            T Amplitude
        ) {
            if (!lut.GetBuffer() || lut.Size() == 0) {
                return false;
            }

            GammaMorph(
                lut.GetBuffer(),
                lut.Size(),
                Morph,
                Gain,
                Amplitude
            );

            return true;
        }

        } // namespace MIDILAR::DspCore::Generators::Shaping

    #endif // MIDILAR_DSP_LUT1D

    #if defined(MIDILAR_DSP_LUT3D)

        #include <DspCore/LUT/LUT3D/LUT3D.h>

        namespace MIDILAR::DspCore::Generators::Shaping {

            template<typename T>
            bool GammaMorphLUT3D(
                MIDILAR::DspCore::LUT::LUT3D<T>& lut,
                float MorphMin,
                float MorphMax,
                float GainMin,
                float GainMax,
                T Amplitude
            ) {
                if (!lut.GetBuffer() || lut.Width() == 0 || lut.Height() == 0 || lut.Depth() == 0) {
                    return false;
                }

                const size_t width  = lut.Width();   // X → input
                const size_t height = lut.Height();  // Y → Morph
                const size_t depth  = lut.Depth();   // Z → Gain

                const float morphStep = (height > 1)
                    ? (MorphMax - MorphMin) / static_cast<float>(height - 1)
                    : 0.0f;

                const float gainStep = (depth > 1)
                    ? (GainMax - GainMin) / static_cast<float>(depth - 1)
                    : 0.0f;

                for (size_t z = 0; z < depth; ++z) {

                    float gain = (depth > 1)
                        ? GainMin + static_cast<float>(z) * gainStep
                        : GainMin;

                    for (size_t y = 0; y < height; ++y) {

                        float morph = (height > 1)
                            ? MorphMin + static_cast<float>(y) * morphStep
                            : MorphMin;

                        // 🔥 cada slice (x) es una curva completa
                        T* slice = lut.GetBuffer() + (z * height + y) * width;

                        GammaMorph(
                            slice,
                            width,
                            morph,
                            gain,
                            Amplitude
                        );
                    }
                }

                return true;
            }

        } // namespace MIDILAR::DspCore::Generators::Shaping

    #endif // MIDILAR_DSP_LUT3D

#endif // MIDILAR_GENERATORS_SHAPING_GAMMA_MORPH_H