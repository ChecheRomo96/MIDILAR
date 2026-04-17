/**
 * @file LUT2D.h
 * @brief Base class for 2D Look-Up Tables (LUTs) used in dspFoundation.
 */

 #ifndef MIDILAR_LUT_2D_H
 #define MIDILAR_LUT_2D_H
 
 #include <stdlib.h>
 #include <math.h>
 #include <stdint.h>
 
 #include <MIDILAR_BuildSettings.h>
 
 namespace MIDILAR::dspFoundation::LUT {
 
     /**
      * @class LUT2D
      * @brief A template-based 2D Look-Up Table for function approximation with two variables.
      * 
      * @tparam OUTPUT_TYPE The data type stored in the LUT.
      */
     template<typename OUTPUT_TYPE>
     class LUT2D {
     public:
         using Type = LUT2D<OUTPUT_TYPE>;
 
     private:
         size_t _width;               ///< Number of columns (X dimension)
         size_t _height;              ///< Number of rows (Y dimension)
         OUTPUT_TYPE* _buffer;        ///< Pointer to flattened 2D buffer
 
         bool _ResizeBuffer(size_t width, size_t height);
 
     protected:
         /**
          * @brief Pure virtual function for evaluating and filling the LUT.
          */
         virtual void Eval() = 0;
 
         OUTPUT_TYPE* GetBuffer();
         const OUTPUT_TYPE* GetBuffer() const;
 
         void SetData(size_t x, size_t y, OUTPUT_TYPE value);
 
         void ClearBuffer();
 
         void SetRawData(OUTPUT_TYPE* data, size_t width, size_t height);
 
     public:
         LUT2D();
         virtual ~LUT2D();
 
         bool SetBufferSize(size_t width, size_t height);
         size_t Width() const;
         size_t Height() const;
 
        virtual OUTPUT_TYPE GetValue(size_t x, size_t y) const;
 
         LUT2D(LUT2D&& other) noexcept;
         LUT2D& operator=(LUT2D&& other) noexcept;
     };
 
 } // namespace MIDILAR::dspFoundation::LUT
 
 #include "LUT2D.tpp"
 
 #endif // MIDILAR_LUT_2D_H
 