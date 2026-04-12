/**
 * @file LUT2D.tpp
 * @brief Implementation of the LUT2D template class.
 */

 #ifndef MIDILAR_LUT_2D_TPP
 #define MIDILAR_LUT_2D_TPP
 
    #include "LUT2D.h"

 namespace MIDILAR::dspFoundation::LUT {
 
     template<typename OUTPUT_TYPE>
     LUT2D<OUTPUT_TYPE>::LUT2D()
         : _width(0), _height(0), _buffer(nullptr) {}
 
     template<typename OUTPUT_TYPE>
     LUT2D<OUTPUT_TYPE>::~LUT2D() {
         if (_buffer) {
             free(_buffer);
             _buffer = nullptr;
         }
     }
 
     template<typename OUTPUT_TYPE>
     bool LUT2D<OUTPUT_TYPE>::_ResizeBuffer(size_t width, size_t height) {
         if (_buffer) {
             free(_buffer);
             _buffer = nullptr;
         }
 
         _width = width;
         _height = height;
         _buffer = static_cast<OUTPUT_TYPE*>(malloc(sizeof(OUTPUT_TYPE) * width * height));
 
         return _buffer != nullptr;
     }
 
     template<typename OUTPUT_TYPE>
     bool LUT2D<OUTPUT_TYPE>::SetBufferSize(size_t width, size_t height) {
         if (_ResizeBuffer(width, height)) {
             Eval();
             return true;
         }
         return false;
     }
 
     template<typename OUTPUT_TYPE>
     size_t LUT2D<OUTPUT_TYPE>::Width() const {
         return _width;
     }
 
     template<typename OUTPUT_TYPE>
     size_t LUT2D<OUTPUT_TYPE>::Height() const {
         return _height;
     }
 
     template<typename OUTPUT_TYPE>
     OUTPUT_TYPE* LUT2D<OUTPUT_TYPE>::GetBuffer() {
         return _buffer;
     }
 
     template<typename OUTPUT_TYPE>
     const OUTPUT_TYPE* LUT2D<OUTPUT_TYPE>::GetBuffer() const {
         return _buffer;
     }
 
     template<typename OUTPUT_TYPE>
     void LUT2D<OUTPUT_TYPE>::SetData(size_t x, size_t y, OUTPUT_TYPE value) {
         if (x < _width && y < _height) {
             _buffer[y * _width + x] = value;
         }
     }
 
     template<typename OUTPUT_TYPE>
     OUTPUT_TYPE LUT2D<OUTPUT_TYPE>::GetValue(size_t x, size_t y) const {
         if (x < _width && y < _height) {
             return _buffer[y * _width + x];
         }
         return static_cast<OUTPUT_TYPE>(0); // Out of bounds safeguard
     }
 
     template<typename OUTPUT_TYPE>
     void LUT2D<OUTPUT_TYPE>::ClearBuffer() {
         if (_buffer) {
             for (size_t i = 0; i < _width * _height; ++i) {
                 _buffer[i] = static_cast<OUTPUT_TYPE>(0);
             }
         }
     }
 
     template<typename OUTPUT_TYPE>
     void LUT2D<OUTPUT_TYPE>::SetRawData(OUTPUT_TYPE* data, size_t width, size_t height) {
         if (_buffer) {
             free(_buffer);
         }
         _width = width;
         _height = height;
         _buffer = data;
     }
 
     template<typename OUTPUT_TYPE>
     LUT2D<OUTPUT_TYPE>::LUT2D(LUT2D&& other) noexcept
         : _width(other._width), _height(other._height), _buffer(other._buffer) {
         other._width = 0;
         other._height = 0;
         other._buffer = nullptr;
     }
 
     template<typename OUTPUT_TYPE>
     LUT2D<OUTPUT_TYPE>& LUT2D<OUTPUT_TYPE>::operator=(LUT2D&& other) noexcept {
         if (this != &other) {
             if (_buffer) {
                 free(_buffer);
             }
             _width = other._width;
             _height = other._height;
             _buffer = other._buffer;
 
             other._width = 0;
             other._height = 0;
             other._buffer = nullptr;
         }
         return *this;
     }
 
 } // namespace MIDILAR::dspFoundation::LUT
 
 #endif // MIDILAR_LUT_2D_TPP
 