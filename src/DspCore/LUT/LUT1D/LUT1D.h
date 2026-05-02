#ifndef MIDILAR_LUT_1D_H
#define MIDILAR_LUT_1D_H

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include <MIDILAR_BuildSettings.h>

namespace MIDILAR::DspCore::LUT {

/**
 * @class LUT1D
 * @brief Lightweight one-dimensional lookup table container.
 *
 * Owns a dynamically allocated buffer and tracks active size separately from
 * allocated capacity.
 *
 * @tparam T Type stored in the LUT.
 */
template<typename T>
class LUT1D {
public:
    using Type = LUT1D<T>;

private:
    T* _buffer;
    size_t _size;
    size_t _capacity;

    bool _Reallocate(size_t newCapacity);

public:
    /// Constructs an empty LUT.
    LUT1D();

    /// Constructs a LUT with the requested active size.
    explicit LUT1D(size_t size);

    LUT1D(const LUT1D& other) = delete;
    LUT1D& operator=(const LUT1D& other) = delete;

    /// Moves another LUT into this one.
    LUT1D(LUT1D&& other) noexcept;

    /// Moves another LUT into this one.
    LUT1D& operator=(LUT1D&& other) noexcept;

    /// Releases the owned buffer.
    ~LUT1D() noexcept;

    /**
     * @brief Reserves memory without changing the active size.
     * @return true if the requested capacity is available.
     */
    bool Reserve(size_t newCapacity);

    /**
     * @brief Changes the active LUT size.
     * @return true if the resize succeeds.
     */
    bool Resize(size_t newSize);

    /// Compatibility alias for Resize().
    bool SetBufferSize(size_t size);

    /// Returns the number of active bins.
    size_t Size() const;

    /// Compatibility alias for Size().
    size_t BufferSize() const;

    /// Returns the allocated capacity.
    size_t Capacity() const;

    /// Returns the mutable internal buffer.
    T* GetBuffer();

    /// Returns the const internal buffer.
    const T* GetBuffer() const;

    /// Returns the value at bin, or zero if bin is out of range.
    T GetValue(size_t bin) const;

    /// Writes value to bin. Out-of-range writes are ignored.
    void SetBinValue(size_t index, T value);

    /// Compatibility alias for SetBinValue().
    void SetData(size_t index, T value);

    /// Clears active bins to zero.
    void Clear();

    /// Compatibility alias for Clear().
    void ClearBuffer();

    /**
     * @brief Copies external data into the LUT.
     *
     * The LUT resizes to bufferSize and copies the data. Ownership of data is
     * not transferred.
     *
     * @return true if the copy succeeds.
     */
    bool SetRawData(const T* data, size_t bufferSize);
};

} // namespace MIDILAR::DspCore::LUT

#include "LUT1D.tpp"

#endif // MIDILAR_LUT_1D_H