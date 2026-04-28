#ifndef MIDILAR_LUT_2D_H
#define MIDILAR_LUT_2D_H

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include <MIDILAR_BuildSettings.h>

namespace MIDILAR::dspFoundation::LUT {

/**
 * @class LUT2D
 * @brief Lightweight two-dimensional lookup table container.
 *
 * Owns a flattened 2D buffer and provides indexed access using x/y coordinates.
 *
 * @tparam T Type stored in the LUT.
 */
template<typename T>
class LUT2D {
public:
    using Type = LUT2D<T>;

private:
    T* _buffer;
    size_t _width;
    size_t _height;
    size_t _capacity;

    bool _Reallocate(size_t newCapacity);

public:
    /// Constructs an empty LUT.
    LUT2D();

    /// Constructs a LUT with the requested dimensions.
    LUT2D(size_t width, size_t height);

    LUT2D(const LUT2D& other) = delete;
    LUT2D& operator=(const LUT2D& other) = delete;

    /// Moves another LUT into this one.
    LUT2D(LUT2D&& other) noexcept;

    /// Moves another LUT into this one.
    LUT2D& operator=(LUT2D&& other) noexcept;

    /// Releases the owned buffer.
    ~LUT2D() noexcept;

    /**
     * @brief Reserves memory without changing active dimensions.
     * @return true if the requested capacity is available.
     */
    bool Reserve(size_t newCapacity);

    /**
     * @brief Changes active dimensions.
     * @return true if resizing succeeds.
     */
    bool Resize(size_t width, size_t height);

    /// Compatibility alias for Resize().
    bool SetBufferSize(size_t width, size_t height);

    /// Returns width dimension.
    size_t Width() const;

    /// Returns height dimension.
    size_t Height() const;

    /// Returns active element count.
    size_t Size() const;

    /// Returns allocated capacity.
    size_t Capacity() const;

    /// Returns mutable internal flattened buffer.
    T* GetBuffer();

    /// Returns const internal flattened buffer.
    const T* GetBuffer() const;

    /// Returns value at x/y, or zero if out of range.
    T GetValue(size_t x, size_t y) const;

    /// Writes value at x/y. Out-of-range writes are ignored.
    void SetBinValue(size_t x, size_t y, T value);

    /// Compatibility alias for SetBinValue().
    void SetData(size_t x, size_t y, T value);

    /// Clears active elements to zero.
    void Clear();

    /// Compatibility alias for Clear().
    void ClearBuffer();

    /**
     * @brief Copies external data into the LUT.
     *
     * The LUT resizes to width x height and copies the data.
     * Ownership of data is not transferred.
     *
     * @return true if the copy succeeds.
     */
    bool SetRawData(const T* data, size_t width, size_t height);
};

} // namespace MIDILAR::dspFoundation::LUT

#include "LUT2D.tpp"

#endif // MIDILAR_LUT_2D_H