/**
 * @file MIDILAR_LookUpTable.h
 * @brief Base class for Look-Up Tables (LUTs) used in AudioFoundation.
 */

 #ifndef MIDILAR_LUT_2D_H
 #define MIDILAR_LUT_2D_H
 
 #include <System/BuildSettings.h>
 #include <stdlib.h> // For malloc/free
 #include <math.h>
 #include <stdint.h>
 
 namespace MIDILAR::AudioFoundation::LUT2D {
 
     ///////////////////////////////////////////////////////////////////////////////////////////////
     /**
      * @class LUT2D
      * @brief A template-based 2D Look-Up Table (LUT) for efficient function approximation.
      * 
      * This class provides a base implementation for LUT-based interpolation.
      * Derived classes should override the `Eval()` method to populate the LUT.
      *
      * @tparam X_TYPE The type used for the X-axis input.
      * @tparam Y_TYPE The type used for the Y-axis input.
      * @tparam OUTPUT_TYPE The data type stored in the LUT.
      * @tparam RANGE_TYPE The range type used for indexing the LUT (default is float).
      */
     template<typename X_TYPE = float, typename Y_TYPE = float, typename OUTPUT_TYPE = float>
     class LUT2D {
         
        public:
            enum InterpolationMode : uint8_t {
                Disabled = 0b00,
                Bilinear = 0b11,
                LinearX = 0b01,
                LinearY = 0b10,
            };
    
        private:
    
            size_t _sizeX;             ///< The number of elements along the X-axis.
            size_t _sizeY;             ///< The number of elements along the Y-axis.
                
            X_TYPE _XRangeMin;  ///< Minimum input range for X.
            X_TYPE _XRangeMax;  ///< Maximum input range for X.
    
            Y_TYPE _YRangeMin;  ///< Minimum input range for Y.
            Y_TYPE _YRangeMax;  ///< Maximum input range for Y.
    
            InterpolationMode _InterpolationSetup;
    
        protected:
    
            OUTPUT_TYPE* _buffer;  ///< Pointer to the allocated LUT buffer.
    
            inline size_t SizeX() const { return _sizeX; }
            inline size_t SizeY() const { return _sizeY; }
    
            inline float InputMinX() const { return _XRangeMin; }
            inline float InputMaxX() const { return _XRangeMax; }
            inline float InputMinY() const { return _YRangeMin; }
            inline float InputMaxY() const { return _YRangeMax; }
    
            bool _ResizeBuffer(size_t newSizeX, size_t newSizeY) {
                if (newSizeX == 0 || newSizeY == 0) return false; // Prevent zero allocation
    
                OUTPUT_TYPE* newBuffer = static_cast<OUTPUT_TYPE*>(malloc(newSizeX * newSizeY * sizeof(OUTPUT_TYPE)));
                if (!newBuffer) return false; // Allocation failed
    
                if (_buffer) {
                    free(_buffer);
                }
    
                _buffer = newBuffer;
                _sizeX = newSizeX;
                _sizeY = newSizeY;
                
                Eval(); // Ensure buffer is recalculated

                return true;
            }
    
            virtual void Eval() = 0;
    
        public:
    
            LUT2D()
                : _buffer(nullptr), _sizeX(0), _sizeY(0),
                _XRangeMin(0.0f), _XRangeMax(1.0f),
                _YRangeMin(0.0f), _YRangeMax(1.0f),
                _InterpolationSetup(InterpolationMode::Disabled) {}
    
            virtual ~LUT2D() {
                if (_buffer) {
                    free(_buffer);
                }
            }
    
            void SetInputRange(float minX, float maxX, float minY, float maxY) {
                if (minX == _XRangeMin && maxX == _XRangeMax &&
                    minY == _YRangeMin && maxY == _YRangeMax) return;
    
                _XRangeMin = minX;
                _XRangeMax = maxX;
                _YRangeMin = minY;
                _YRangeMax = maxY;
                Eval(); // Ensure buffer is recalculated
            }
    
            OUTPUT_TYPE GetValue(X_TYPE x, Y_TYPE y) const {
                if (!_buffer || _sizeX == 0 || _sizeY == 0) {
                    return OUTPUT_TYPE(); // Return default if buffer is uninitialized
                }
    
                float normX = (x - _XRangeMin) / (_XRangeMax - _XRangeMin);
                float normY = (y - _YRangeMin) / (_YRangeMax - _YRangeMin);
    
                normX = fmaxf(0.0f, fminf(normX, 1.0f));
                normY = fmaxf(0.0f, fminf(normY, 1.0f));
    
                size_t indexX = static_cast<size_t>(normX * (_sizeX - 1));
                size_t indexY = static_cast<size_t>(normY * (_sizeY - 1));
                
                switch (_InterpolationSetup) {
                    case InterpolationMode::Bilinear: {
                        // Ensure bounds
                        size_t nextX = (indexX + 1 < _sizeX) ? indexX + 1 : indexX;
                        size_t nextY = (indexY + 1 < _sizeY) ? indexY + 1 : indexY;
                
                        float fx = fmaxf(0.0f, fminf(normX * (_sizeX - 1) - indexX, 1.0f));
                        float fy = fmaxf(0.0f, fminf(normY * (_sizeY - 1) - indexY, 1.0f));
                
                        OUTPUT_TYPE v00 = _buffer[indexY * _sizeX + indexX];
                        OUTPUT_TYPE v10 = _buffer[indexY * _sizeX + nextX];
                        OUTPUT_TYPE v01 = _buffer[nextY * _sizeX + indexX];
                        OUTPUT_TYPE v11 = _buffer[nextY * _sizeX + nextX];
                
                        return (1.0f - fx) * (1.0f - fy) * v00 +
                            fx * (1.0f - fy) * v10 +
                            (1.0f - fx) * fy * v01 +
                            fx * fy * v11;
                    }
                
                    case InterpolationMode::LinearX: {
                        size_t nextX = (indexX + 1 < _sizeX) ? indexX + 1 : indexX;
                        float fx = fmaxf(0.0f, fminf(normX * (_sizeX - 1) - indexX, 1.0f));
                
                        OUTPUT_TYPE v0 = _buffer[indexY * _sizeX + indexX];
                        OUTPUT_TYPE v1 = _buffer[indexY * _sizeX + nextX];
                
                        return v0 + fx * (v1 - v0);
                    }
                
                    case InterpolationMode::LinearY: {
                        size_t nextY = (indexY + 1 < _sizeY) ? indexY + 1 : indexY;
                        float fy = fmaxf(0.0f, fminf(normY * (_sizeY - 1) - indexY, 1.0f));
                
                        OUTPUT_TYPE v0 = _buffer[indexY * _sizeX + indexX];
                        OUTPUT_TYPE v1 = _buffer[nextY * _sizeX + indexX];
                
                        return v0 + fy * (v1 - v0);
                    }
                
                    case InterpolationMode::Disabled:
                        return _buffer[indexY * _sizeX + indexX];
                
                    default:
                        return OUTPUT_TYPE(); // Return default value if an invalid mode is set
                }
            

             
         }
     };
 
 } // namespace MIDILAR::AudioFoundation::LUT
 
 #endif // MIDILAR_LUT_2D_H
 