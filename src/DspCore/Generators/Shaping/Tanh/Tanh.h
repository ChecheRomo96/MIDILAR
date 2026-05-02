#ifndef MIDILAR_GENERATORS_TANH_H
#define MIDILAR_GENERATORS_TANH_H

    #include <MIDILAR_BuildSettings.h>
    #include <math.h>
    #include <stddef.h>

    namespace MIDILAR::DspCore::Generators::Shaping {

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @brief Generates a 1D raw hyperbolic tangent shaping curve.
         *
         * Fills the given buffer with the raw tanh curve:
         * \f[
         * y(x) = \tanh(a x)
         * \f]
         * where:
         * \f[
         * x \in [-1, 1], \quad a \in (0, \infty)
         * \f]
         *
         * This version is not amplitude-normalized. Therefore, for finite values of Drive:
         * \f[
         * y(1) = \tanh(a)
         * \f]
         *
         * @tparam T The output type.
         * @param Buffer Output buffer to write the curve into.
         * @param BufferSize Number of samples.
         * @param Drive Controls the curve saturation strength.
         * @param OutputScale Output scaling factor. Use 1.0f for normalized floating-point output, or values such as 32767 for signed integer LUTs.
         */
        template<typename T>
        void Tanh(
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
                const float y = tanhf(Drive * x);

                Buffer[i] = static_cast<T>(
                    y * static_cast<float>(OutputScale)
                );
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @brief Generates a 1D normalized hyperbolic tangent shaping curve.
         *
         * Fills the given buffer with the normalized tanh curve:
         * \f[
         * y(x) = \frac{\tanh(a x)}{\tanh(a)}
         * \f]
         * where:
         * \f[
         * x \in [-1, 1], \quad a \in (0, \infty)
         * \f]
         *
         * This guarantees:
         * \f[
         * y(-1) = -1, \quad y(0) = 0, \quad y(1) = 1
         * \f]
         *
         * @tparam T The output type.
         * @param Buffer Output buffer to write the curve into.
         * @param BufferSize Number of samples.
         * @param Drive Controls the curve saturation strength.
         * @param OutputScale Output scaling factor. Use 1.0f for normalized floating-point output, or values such as 32767 for signed integer LUTs.
         */
        template<typename T>
        void TanhNorm(
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
            const float normalization = tanhf(Drive);

            for (size_t i = 0; i < BufferSize; ++i) {
                const float x = -1.0f + static_cast<float>(i) * xStep;
                const float y = tanhf(Drive * x) / normalization;

                Buffer[i] = static_cast<T>(
                    y * static_cast<float>(OutputScale)
                );
            }

            Buffer[0] = static_cast<T>(-static_cast<float>(OutputScale));
            Buffer[BufferSize - 1] = OutputScale;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @brief Generates the positive half of a raw hyperbolic tangent shaping curve.
         *
         * Fills the given buffer with the raw positive tanh curve:
         * \f[
         * y(x) = \tanh(a x)
         * \f]
         * where:
         * \f[
         * x \in [0, 1], \quad a \in (0, \infty)
         * \f]
         *
         * Intended evaluation for bipolar input:
         * \f[
         * y(x) = \operatorname{sign}(x) \cdot LUT(|x|)
         * \f]
         *
         * @tparam T The output type.
         * @param Buffer Output buffer to write the positive half-curve into.
         * @param BufferSize Number of samples.
         * @param Drive Controls the curve saturation strength.
         * @param OutputScale Output scaling factor. Use 1.0f for normalized floating-point output, or values such as 32767 for signed integer LUTs.
         */
        template<typename T>
        void TanhHalf(
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
                const float y = tanhf(Drive * x);

                Buffer[i] = static_cast<T>(
                    y * static_cast<float>(OutputScale)
                );
            }

            Buffer[0] = static_cast<T>(0);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @brief Generates the positive half of a normalized hyperbolic tangent shaping curve.
         *
         * Fills the given buffer with the normalized positive tanh curve:
         * \f[
         * y(x) = \frac{\tanh(a x)}{\tanh(a)}
         * \f]
         * where:
         * \f[
         * x \in [0, 1], \quad a \in (0, \infty)
         * \f]
         *
         * This guarantees:
         * \f[
         * y(0) = 0, \quad y(1) = 1
         * \f]
         *
         * Intended evaluation for bipolar input:
         * \f[
         * y(x) = \operatorname{sign}(x) \cdot LUT(|x|)
         * \f]
         *
         * @tparam T The output type.
         * @param Buffer Output buffer to write the positive half-curve into.
         * @param BufferSize Number of samples.
         * @param Drive Controls the curve saturation strength.
         * @param OutputScale Output scaling factor. Use 1.0f for normalized floating-point output, or values such as 32767 for signed integer LUTs.
         */
        template<typename T>
        void TanhHalfNorm(
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
            const float normalization = tanhf(Drive);

            for (size_t i = 0; i < BufferSize; ++i) {
                const float x = static_cast<float>(i) * xStep;
                const float y = tanhf(Drive * x) / normalization;

                Buffer[i] = static_cast<T>(
                    y * static_cast<float>(OutputScale)
                );
            }

            Buffer[0] = static_cast<T>(0);
            Buffer[BufferSize - 1] = OutputScale;
        }

    } // namespace MIDILAR::DspCore::Generators::Shaping

    #if defined(MIDILAR_DSP_LUT1D)

        #include <DspCore/LUT/LUT1D.h>

        namespace MIDILAR::DspCore::Generators::Shaping {

            template<typename T>
            bool TanhLUT(
                MIDILAR::DspCore::LUT::LUT1D<T>& lut,
                float Drive,
                T OutputScale = static_cast<T>(1.0f)
            ) {
                if (!lut.GetBuffer() || lut.Size() == 0) {
                    return false;
                }

                Tanh<T>(
                    lut.GetBuffer(),
                    lut.Size(),
                    Drive,
                    OutputScale
                );

                return true;
            }

            template<typename T>
            bool TanhNormLUT(
                MIDILAR::DspCore::LUT::LUT1D<T>& lut,
                float Drive,
                T OutputScale = static_cast<T>(1.0f)
            ) {
                if (!lut.GetBuffer() || lut.Size() == 0) {
                    return false;
                }

                TanhNorm<T>(
                    lut.GetBuffer(),
                    lut.Size(),
                    Drive,
                    OutputScale
                );

                return true;
            }

            template<typename T>
            bool TanhHalfLUT(
                MIDILAR::DspCore::LUT::LUT1D<T>& lut,
                float Drive,
                T OutputScale = static_cast<T>(1.0f)
            ) {
                if (!lut.GetBuffer() || lut.Size() == 0) {
                    return false;
                }

                TanhHalf<T>(
                    lut.GetBuffer(),
                    lut.Size(),
                    Drive,
                    OutputScale
                );

                return true;
            }

            template<typename T>
            bool TanhHalfNormLUT(
                MIDILAR::DspCore::LUT::LUT1D<T>& lut,
                float Drive,
                T OutputScale = static_cast<T>(1.0f)
            ) {
                if (!lut.GetBuffer() || lut.Size() == 0) {
                    return false;
                }

                TanhHalfNorm<T>(
                    lut.GetBuffer(),
                    lut.Size(),
                    Drive,
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
            bool TanhLUT2D(
                MIDILAR::DspCore::LUT::LUT2D<T>& lut,
                float DriveMin,
                float DriveMax,
                T OutputScale = static_cast<T>(1.0f)
            ) {
                if (!lut.GetBuffer() || lut.Width() == 0 || lut.Height() == 0) {
                    return false;
                }

                const size_t width  = lut.Width();
                const size_t height = lut.Height();

                const float driveStep = (height > 1)
                    ? (DriveMax - DriveMin) / static_cast<float>(height - 1)
                    : 0.0f;

                for (size_t y = 0; y < height; ++y) {
                    const float driveValue = (height > 1)
                        ? DriveMin + static_cast<float>(y) * driveStep
                        : DriveMin;

                    T* rowBuffer = lut.GetBuffer() + y * width;

                    Tanh<T>(
                        rowBuffer,
                        width,
                        driveValue,
                        OutputScale
                    );
                }

                return true;
            }

            template<typename T>
            bool TanhNormLUT2D(
                MIDILAR::DspCore::LUT::LUT2D<T>& lut,
                float DriveMin,
                float DriveMax,
                T OutputScale = static_cast<T>(1.0f)
            ) {
                if (!lut.GetBuffer() || lut.Width() == 0 || lut.Height() == 0) {
                    return false;
                }

                const size_t width  = lut.Width();
                const size_t height = lut.Height();

                const float driveStep = (height > 1)
                    ? (DriveMax - DriveMin) / static_cast<float>(height - 1)
                    : 0.0f;

                for (size_t y = 0; y < height; ++y) {
                    const float driveValue = (height > 1)
                        ? DriveMin + static_cast<float>(y) * driveStep
                        : DriveMin;

                    T* rowBuffer = lut.GetBuffer() + y * width;

                    TanhNorm<T>(
                        rowBuffer,
                        width,
                        driveValue,
                        OutputScale
                    );
                }

                return true;
            }

            template<typename T>
            bool TanhHalfLUT2D(
                MIDILAR::DspCore::LUT::LUT2D<T>& lut,
                float DriveMin,
                float DriveMax,
                T OutputScale = static_cast<T>(1.0f)
            ) {
                if (!lut.GetBuffer() || lut.Width() == 0 || lut.Height() == 0) {
                    return false;
                }

                const size_t width  = lut.Width();
                const size_t height = lut.Height();

                const float driveStep = (height > 1)
                    ? (DriveMax - DriveMin) / static_cast<float>(height - 1)
                    : 0.0f;

                for (size_t y = 0; y < height; ++y) {
                    const float driveValue = (height > 1)
                        ? DriveMin + static_cast<float>(y) * driveStep
                        : DriveMin;

                    T* rowBuffer = lut.GetBuffer() + y * width;

                    TanhHalf<T>(
                        rowBuffer,
                        width,
                        driveValue,
                        OutputScale
                    );
                }

                return true;
            }

            template<typename T>
            bool TanhHalfNormLUT2D(
                MIDILAR::DspCore::LUT::LUT2D<T>& lut,
                float DriveMin,
                float DriveMax,
                T OutputScale = static_cast<T>(1.0f)
            ) {
                if (!lut.GetBuffer() || lut.Width() == 0 || lut.Height() == 0) {
                    return false;
                }

                const size_t width  = lut.Width();
                const size_t height = lut.Height();

                const float driveStep = (height > 1)
                    ? (DriveMax - DriveMin) / static_cast<float>(height - 1)
                    : 0.0f;

                for (size_t y = 0; y < height; ++y) {
                    const float driveValue = (height > 1)
                        ? DriveMin + static_cast<float>(y) * driveStep
                        : DriveMin;

                    T* rowBuffer = lut.GetBuffer() + y * width;

                    TanhHalfNorm<T>(
                        rowBuffer,
                        width,
                        driveValue,
                        OutputScale
                    );
                }

                return true;
            }

        } // namespace MIDILAR::DspCore::Generators::Shaping

    #endif // MIDILAR_DSP_LUT2D

#endif // MIDILAR_GENERATORS_TANH_H
