#ifndef MIDILAR_DSP_TRANSFORMS_DFT_H
#define MIDILAR_DSP_TRANSFORMS_DFT_H

#include <math.h>
#include <stddef.h>

namespace MIDILAR::DspCore::Transforms {

    /**
     * @brief Computes the forward Discrete Fourier Transform.
     *
     * Computes:
     * \f[
     * X[k] = \sum_{n=0}^{N-1} x[n] e^{-j 2\pi kn/N}
     * \f]
     *
     * The result is written into separate real and imaginary output buffers.
     *
     * @tparam TIn  Input sample type.
     * @tparam TOut Output sample type.
     *
     * @param realIn  Pointer to the real input signal.
     * @param imagIn  Pointer to the imaginary input signal. May be nullptr.
     * @param realOut Pointer to the output real spectrum buffer.
     * @param imagOut Pointer to the output imaginary spectrum buffer. May be nullptr.
     * @param size Number of samples.
     */
    template<typename TIn, typename TOut = float>
    void DFT(
        const TIn* realIn,
        const TIn* imagIn,
        TOut* realOut,
        TOut* imagOut,
        size_t size
    ) {
        if ((realIn == nullptr) ||
            (realOut == nullptr) ||
            (size == 0)) {
            return;
        }

        for (size_t k = 0; k < size; ++k) {

            double sumReal = 0.0;
            double sumImag = 0.0;

            for (size_t n = 0; n < size; ++n) {

                const double angle =
                    -2.0 *
                    M_PI *
                    static_cast<double>(k) *
                    static_cast<double>(n) /
                    static_cast<double>(size);

                const double inReal = static_cast<double>(realIn[n]);
                const double inImag = (imagIn != nullptr)
                    ? static_cast<double>(imagIn[n])
                    : 0.0;

                const double cosVal = cos(angle);
                const double sinVal = sin(angle);

                sumReal += (inReal * cosVal) - (inImag * sinVal);
                sumImag += (inReal * sinVal) + (inImag * cosVal);
            }

            realOut[k] = static_cast<TOut>(sumReal);

            if (imagOut != nullptr) {
                imagOut[k] = static_cast<TOut>(sumImag);
            }
        }
    }

    /**
     * @brief Computes the inverse Discrete Fourier Transform.
     *
     * Computes:
     * \f[
     * x[n] = \frac{1}{N} \sum_{k=0}^{N-1} X[k] e^{j 2\pi kn/N}
     * \f]
     *
     * The result is written into separate real and imaginary output buffers.
     *
     * @tparam TIn  Input spectrum type.
     * @tparam TOut Output sample type.
     *
     * @param realIn Pointer to the real spectrum buffer.
     * @param imagIn Pointer to the imaginary spectrum buffer. May be nullptr.
     * @param realOut Pointer to the reconstructed real signal buffer.
     * @param imagOut Pointer to the reconstructed imaginary signal buffer. May be nullptr.
     * @param size Number of samples.
     */
    template<typename TIn, typename TOut = float>
    void IDFT(
        const TIn* realIn,
        const TIn* imagIn,
        TOut* realOut,
        TOut* imagOut,
        size_t size
    ) {
        if ((realIn == nullptr) ||
            (realOut == nullptr) ||
            (size == 0)) {
            return;
        }

        for (size_t n = 0; n < size; ++n) {

            double sumReal = 0.0;
            double sumImag = 0.0;

            for (size_t k = 0; k < size; ++k) {

                const double angle =
                    2.0 *
                    M_PI *
                    static_cast<double>(k) *
                    static_cast<double>(n) /
                    static_cast<double>(size);

                const double specReal = static_cast<double>(realIn[k]);
                const double specImag = (imagIn != nullptr)
                    ? static_cast<double>(imagIn[k])
                    : 0.0;

                const double cosVal = cos(angle);
                const double sinVal = sin(angle);

                sumReal += (specReal * cosVal) - (specImag * sinVal);
                sumImag += (specReal * sinVal) + (specImag * cosVal);
            }

            realOut[n] = static_cast<TOut>(
                sumReal / static_cast<double>(size)
            );

            if (imagOut != nullptr) {
                imagOut[n] = static_cast<TOut>(
                    sumImag / static_cast<double>(size)
                );
            }
        }
    }

} // namespace MIDILAR::DspCore::Transforms

#endif // MIDILAR_DSP_TRANSFORMS_DFT_H