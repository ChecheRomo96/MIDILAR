#ifndef MIDILAR_LUT_3D_TPP
#define MIDILAR_LUT_3D_TPP

#include "LUT3D.h"

namespace MIDILAR::dspFoundation::LUT {

    template<typename OUTPUT_TYPE>
    LUT3D<OUTPUT_TYPE>::LUT3D()
        : _width(0), _height(0), _depth(0), _buffer(nullptr) {}

    template<typename OUTPUT_TYPE>
    LUT3D<OUTPUT_TYPE>::~LUT3D() {
        if (_buffer) {
            free(_buffer);
        }
    }

    template<typename OUTPUT_TYPE>
    bool LUT3D<OUTPUT_TYPE>::_ResizeBuffer(size_t width, size_t height, size_t depth) {
        if (width == 0 || height == 0 || depth == 0) {
            if (_buffer) {
                free(_buffer);
            }
            _width = _height = _depth = 0;
            return true;
        }

        size_t totalSize = width * height * depth;
        OUTPUT_TYPE* newBuffer = static_cast<OUTPUT_TYPE*>(malloc(totalSize * sizeof(OUTPUT_TYPE)));
        if (!newBuffer) return false;

        if (_buffer) {
            free(_buffer);
        }

        _buffer = newBuffer;
        _width = width;
        _height = height;
        _depth = depth;
        return true;
    }

    template<typename OUTPUT_TYPE>
    bool LUT3D<OUTPUT_TYPE>::SetBufferSize(size_t width, size_t height, size_t depth) {
        if (width == _width && height == _height && depth == _depth) return true;
        if (_ResizeBuffer(width, height, depth)) {
            Eval();
            return true;
        }
        return false;
    }

    template<typename OUTPUT_TYPE>
    OUTPUT_TYPE* LUT3D<OUTPUT_TYPE>::GetBuffer() {
        return _buffer;
    }

    template<typename OUTPUT_TYPE>
    const OUTPUT_TYPE* LUT3D<OUTPUT_TYPE>::GetBuffer() const {
        return _buffer;
    }

    template<typename OUTPUT_TYPE>
    void LUT3D<OUTPUT_TYPE>::SetData(size_t x, size_t y, size_t z, OUTPUT_TYPE value) {
        if (x < _width && y < _height && z < _depth) {
            _buffer[(z * _height + y) * _width + x] = value;
        }
    }

    template<typename OUTPUT_TYPE>
    OUTPUT_TYPE LUT3D<OUTPUT_TYPE>::GetValue(size_t x, size_t y, size_t z) const {
        if (x < _width && y < _height && z < _depth) {
            return _buffer[(z * _height + y) * _width + x];
        }
        return static_cast<OUTPUT_TYPE>(0);
    }
    
    template<typename OUTPUT_TYPE>
    OUTPUT_TYPE LUT3D<OUTPUT_TYPE>::GetValue(size_t x, size_t y) const {
        return GetValue(x, y, 0);
    }
    
    template<typename OUTPUT_TYPE>
    OUTPUT_TYPE LUT3D<OUTPUT_TYPE>::GetValue(size_t x) const {
        return GetValue(x, 0, 0);
    }

    template<typename OUTPUT_TYPE>
    void LUT3D<OUTPUT_TYPE>::ClearBuffer() {
        size_t totalSize = _width * _height * _depth;
        for (size_t i = 0; i < totalSize; ++i) {
            _buffer[i] = OUTPUT_TYPE();
        }
    }

    template<typename OUTPUT_TYPE>
    void LUT3D<OUTPUT_TYPE>::SetRawData(OUTPUT_TYPE* data, size_t width, size_t height, size_t depth) {
        if (!data || width == 0 || height == 0 || depth == 0) return;

        if (!_ResizeBuffer(width, height, depth)) return;

        size_t totalSize = width * height * depth;
        for (size_t i = 0; i < totalSize; ++i) {
            _buffer[i] = data[i];
        }
    }

    template<typename OUTPUT_TYPE>
    size_t LUT3D<OUTPUT_TYPE>::Width() const {
        return _width;
    }

    template<typename OUTPUT_TYPE>
    size_t LUT3D<OUTPUT_TYPE>::Height() const {
        return _height;
    }

    template<typename OUTPUT_TYPE>
    size_t LUT3D<OUTPUT_TYPE>::Depth() const {
        return _depth;
    }

    template<typename OUTPUT_TYPE>
    LUT3D<OUTPUT_TYPE>::LUT3D(LUT3D&& other) noexcept
        : _width(other._width), _height(other._height), _depth(other._depth), _buffer(other._buffer) {
        other._width = other._height = other._depth = 0;
        other._buffer = nullptr;
    }

    template<typename OUTPUT_TYPE>
    LUT3D<OUTPUT_TYPE>& LUT3D<OUTPUT_TYPE>::operator=(LUT3D&& other) noexcept {
        if (this != &other) {
            free(_buffer);
            _width = other._width;
            _height = other._height;
            _depth = other._depth;
            _buffer = other._buffer;
            other._width = other._height = other._depth = 0;
            other._buffer = nullptr;
        }
        return *this;
    }

} // namespace MIDILAR::dspFoundation::LUT

#endif // MIDILAR_LUT_3D_TPP
