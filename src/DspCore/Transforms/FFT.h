#ifndef MIDILAR_DSP_TRANSFORMS_FFT_H
#define MIDILAR_DSP_TRANSFORMS_FFT_H

#include <math.h>
#include <stddef.h>
#include <stdint.h>

namespace MIDILAR::DspCore::Transforms {

    /**
     * @brief Returns true if the given value is a power of two.
     *
     * @param value Value to test.
     * @return True if value is a power of two and nonzero.
     */
    inline bool IsPowerOfTwo(size_t value) {
        return (value != 0) && ((value & (value - 1)) == 0);
    }

    /**
     * @brief Reverses the lowest @p bitCount bits of @p value.
     *
     * @param value Input value.
     * @param bitCount Number of bits to reverse.
     * @return Bit-reversed value.
     */
    inline size_t ReverseBits(size_t value, size_t bitCount) {
        size_t reversed = 0;

        for (size_t i = 0; i < bitCount; ++i) {
            reversed = (reversed << 1) | (value & 1);
            value >>= 1;
        }

        return reversed;
    }

    /**
     * @brief Computes the base-2 logarithm of a power-of-two size.
     *
     * @param value Input value. Must be a power of two.
     * @return log2(value)
     */
    inline size_t Log2Size(size_t value) {
        size_t count = 0;

        while (value > 1) {
            value >>= 1;
            ++count;
        }

        return count;
    }

    /**
     * @brief Computes the forward Fast Fourier Transform using radix-2 Cooley-Tukey.
     *
     * The input is copied into the output buffers using bit-reversed ordering, and
     * the transform is computed in-place on the output buffers.
     *
     * @tparam TIn Input sample type.
     * @tparam TOut Output sample type.
     *
     * @param realIn Pointer to the real input signal.
     * @param imagIn Pointer to the imaginary input signal. May be nullptr.
     * @param realOut Pointer to the real output spectrum buffer.
     * @param imagOut Pointer to the imaginary output spectrum buffer.
     * @param size Number of samples. Must be a power of two.
     */
    template<typename TIn, typename TOut = float>
    void FFT(
        const TIn* realIn,
        const TIn* imagIn,
        TOut* realOut,
        TOut* imagOut,
        size_t size
    ) {
        if ((realIn == nullptr) ||
            (realOut == nullptr) ||
            (imagOut == nullptr) ||
            (size == 0) ||
            (!IsPowerOfTwo(size))) {
            return;
        }

        const size_t bitCount = Log2Size(size);

        // Bit-reversal copy
        for (size_t i = 0; i < size; ++i) {
            const size_t j = ReverseBits(i, bitCount);

            realOut[j] = static_cast<TOut>(realIn[i]);
            imagOut[j] = static_cast<TOut>((imagIn != nullptr) ? imagIn[i] : 0);
        }

        // FFT stages
        for (size_t blockSize = 2; blockSize <= size; blockSize <<= 1) {
            const size_t halfBlock = blockSize >> 1;
            const double angleStep = -2.0 * M_PI / static_cast<double>(blockSize);

            for (size_t blockStart = 0; blockStart < size; blockStart += blockSize) {
                for (size_t j = 0; j < halfBlock; ++j) {
                    const double angle = angleStep * static_cast<double>(j);
                    const double wReal = cos(angle);
                    const double wImag = sin(angle);

                    const size_t index0 = blockStart + j;
                    const size_t index1 = index0 + halfBlock;

                    const double uReal = static_cast<double>(realOut[index0]);
                    const double uImag = static_cast<double>(imagOut[index0]);

                    const double vReal = static_cast<double>(realOut[index1]);
                    const double vImag = static_cast<double>(imagOut[index1]);

                    const double tReal = (wReal * vReal) - (wImag * vImag);
                    const double tImag = (wReal * vImag) + (wImag * vReal);

                    realOut[index0] = static_cast<TOut>(uReal + tReal);
                    imagOut[index0] = static_cast<TOut>(uImag + tImag);

                    realOut[index1] = static_cast<TOut>(uReal - tReal);
                    imagOut[index1] = static_cast<TOut>(uImag - tImag);
                }
            }
        }
    }

} // namespace MIDILAR::DspCore::Transforms

#endif // MIDILAR_DSP_TRANSFORMS_FFT_H