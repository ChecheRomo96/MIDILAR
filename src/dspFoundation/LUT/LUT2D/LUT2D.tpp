#ifndef MIDILAR_LUT_2D_TPP
#define MIDILAR_LUT_2D_TPP

#include "LUT2D.h"

namespace MIDILAR::dspFoundation::LUT {

//=============================================================
// Private
//=============================================================

template<typename T>
bool LUT2D<T>::_Reallocate(size_t newCapacity) {
    if (newCapacity == _capacity) {
        return true;
    }

    if (newCapacity == 0) {
        if (_buffer) {
            free(_buffer);
        }

        _buffer = nullptr;
        _width = 0;
        _height = 0;
        _capacity = 0;

        return true;
    }

    T* newBuffer = static_cast<T*>(malloc(newCapacity * sizeof(T)));

    if (!newBuffer) {
        return false;
    }

    size_t copySize = Size();

    if (copySize > newCapacity) {
        copySize = newCapacity;
    }

    for (size_t i = 0; i < copySize; ++i) {
        newBuffer[i] = _buffer[i];
    }

    if (_buffer) {
        free(_buffer);
    }

    _buffer = newBuffer;
    _capacity = newCapacity;

    if (Size() > _capacity) {
        _width = _capacity;
        _height = 1;
    }

    return true;
}

//=============================================================
// Constructors / Destructor
//=============================================================

template<typename T>
LUT2D<T>::LUT2D()
    : _buffer(nullptr),
      _width(0),
      _height(0),
      _capacity(0) {}

template<typename T>
LUT2D<T>::LUT2D(size_t width, size_t height)
    : _buffer(nullptr),
      _width(0),
      _height(0),
      _capacity(0) {
    Resize(width, height);
}

template<typename T>
LUT2D<T>::LUT2D(LUT2D&& other) noexcept
    : _buffer(other._buffer),
      _width(other._width),
      _height(other._height),
      _capacity(other._capacity) {
    other._buffer = nullptr;
    other._width = 0;
    other._height = 0;
    other._capacity = 0;
}

template<typename T>
LUT2D<T>& LUT2D<T>::operator=(LUT2D&& other) noexcept {
    if (this != &other) {
        if (_buffer) {
            free(_buffer);
        }

        _buffer = other._buffer;
        _width = other._width;
        _height = other._height;
        _capacity = other._capacity;

        other._buffer = nullptr;
        other._width = 0;
        other._height = 0;
        other._capacity = 0;
    }

    return *this;
}

template<typename T>
LUT2D<T>::~LUT2D() noexcept {
    if (_buffer) {
        free(_buffer);
    }
}

//=============================================================
// Memory management
//=============================================================

template<typename T>
bool LUT2D<T>::Reserve(size_t newCapacity) {
    if (newCapacity <= _capacity) {
        return true;
    }

    return _Reallocate(newCapacity);
}

template<typename T>
bool LUT2D<T>::Resize(size_t width, size_t height) {
    size_t newSize = width * height;

    if (newSize > _capacity) {
        if (!_Reallocate(newSize)) {
            return false;
        }
    }

    _width = width;
    _height = height;

    return true;
}

template<typename T>
bool LUT2D<T>::SetBufferSize(size_t width, size_t height) {
    return Resize(width, height);
}

//=============================================================
// Accessors
//=============================================================

template<typename T>
size_t LUT2D<T>::Width() const {
    return _width;
}

template<typename T>
size_t LUT2D<T>::Height() const {
    return _height;
}

template<typename T>
size_t LUT2D<T>::Size() const {
    return _width * _height;
}

template<typename T>
size_t LUT2D<T>::Capacity() const {
    return _capacity;
}

template<typename T>
T* LUT2D<T>::GetBuffer() {
    return _buffer;
}

template<typename T>
const T* LUT2D<T>::GetBuffer() const {
    return _buffer;
}

//=============================================================
// Data access
//=============================================================

template<typename T>
T LUT2D<T>::GetValue(size_t x, size_t y) const {
    if (!_buffer || x >= _width || y >= _height) {
        return T{};
    }

    return _buffer[(y * _width) + x];
}

template<typename T>
void LUT2D<T>::SetBinValue(size_t x, size_t y, T value) {
    if (!_buffer || x >= _width || y >= _height) {
        return;
    }

    _buffer[(y * _width) + x] = value;
}

template<typename T>
void LUT2D<T>::SetData(size_t x, size_t y, T value) {
    SetBinValue(x, y, value);
}

//=============================================================
// Utilities
//=============================================================

template<typename T>
void LUT2D<T>::Clear() {
    if (!_buffer) {
        return;
    }

    for (size_t i = 0; i < Size(); ++i) {
        _buffer[i] = T{};
    }
}

template<typename T>
void LUT2D<T>::ClearBuffer() {
    Clear();
}

template<typename T>
bool LUT2D<T>::SetRawData(const T* data, size_t width, size_t height) {
    size_t newSize = width * height;

    if (!data && newSize > 0) {
        return false;
    }

    if (!Resize(width, height)) {
        return false;
    }

    for (size_t i = 0; i < newSize; ++i) {
        _buffer[i] = data[i];
    }

    return true;
}

} // namespace MIDILAR::dspFoundation::LUT

#endif // MIDILAR_LUT_2D_TPP