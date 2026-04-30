#ifndef MIDILAR_GENERATORS_ATAN_H
#define MIDILAR_GENERATORS_ATAN_H

    #include <MIDILAR_BuildSettings.h>
    #include <math.h>
    #include <stddef.h>

    namespace MIDILAR::dspFoundation::Generators::Shaping {

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @brief Generates a 1D raw arctangent shaping curve.
         *
         * y(x) = atan(a x)
         */
        template<typename T>
        void Atan(
            T* Buffer,
            size_t BufferSize,
            float Drive,
            T OutputScale = static_cast<T>(1.0f)
        ) {
            if (!Buffer || BufferSize == 0) return;

            if (BufferSize == 1) {
                Buffer[0] = static_cast<T>(0);
                return;
            }

            Drive = fmaxf(Drive, 0.000001f);

            const float xStep = 2.0f / static_cast<float>(BufferSize - 1);

            for (size_t i = 0; i < BufferSize; ++i) {
                const float x = -1.0f + static_cast<float>(i) * xStep;
                const float y = atanf(Drive * x);

                Buffer[i] = static_cast<T>(y * OutputScale);
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @brief Normalized atan curve.
         *
         * y(x) = atan(a x) / atan(a)
         */
        template<typename T>
        void AtanNorm(
            T* Buffer,
            size_t BufferSize,
            float Drive,
            T OutputScale = static_cast<T>(1.0f)
        ) {
            if (!Buffer || BufferSize == 0) return;

            if (BufferSize == 1) {
                Buffer[0] = static_cast<T>(0);
                return;
            }

            Drive = fmaxf(Drive, 0.000001f);

            const float xStep = 2.0f / static_cast<float>(BufferSize - 1);
            const float normalization = atanf(Drive);

            for (size_t i = 0; i < BufferSize; ++i) {
                const float x = -1.0f + static_cast<float>(i) * xStep;
                const float y = atanf(Drive * x) / normalization;

                Buffer[i] = static_cast<T>(y * OutputScale);
            }

            Buffer[0] = static_cast<T>(-OutputScale);
            Buffer[BufferSize - 1] = OutputScale;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @brief Positive half atan.
         */
        template<typename T>
        void AtanHalf(
            T* Buffer,
            size_t BufferSize,
            float Drive,
            T OutputScale = static_cast<T>(1.0f)
        ) {
            if (!Buffer || BufferSize == 0) return;

            if (BufferSize == 1) {
                Buffer[0] = static_cast<T>(0);
                return;
            }

            Drive = fmaxf(Drive, 0.000001f);

            const float xStep = 1.0f / static_cast<float>(BufferSize - 1);

            for (size_t i = 0; i < BufferSize; ++i) {
                const float x = static_cast<float>(i) * xStep;
                const float y = atanf(Drive * x);

                Buffer[i] = static_cast<T>(y * OutputScale);
            }

            Buffer[0] = static_cast<T>(0);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @brief Normalized positive half atan.
         */
        template<typename T>
        void AtanHalfNorm(
            T* Buffer,
            size_t BufferSize,
            float Drive,
            T OutputScale = static_cast<T>(1.0f)
        ) {
            if (!Buffer || BufferSize == 0) return;

            if (BufferSize == 1) {
                Buffer[0] = static_cast<T>(0);
                return;
            }

            Drive = fmaxf(Drive, 0.000001f);

            const float xStep = 1.0f / static_cast<float>(BufferSize - 1);
            const float normalization = atanf(Drive);

            for (size_t i = 0; i < BufferSize; ++i) {
                const float x = static_cast<float>(i) * xStep;
                const float y = atanf(Drive * x) / normalization;

                Buffer[i] = static_cast<T>(y * OutputScale);
            }

            Buffer[0] = static_cast<T>(0);
            Buffer[BufferSize - 1] = OutputScale;
        }

    } // namespace MIDILAR::dspFoundation::Generators::Shaping

#endif//MIDILAR_GENERATORS_ATAN_H