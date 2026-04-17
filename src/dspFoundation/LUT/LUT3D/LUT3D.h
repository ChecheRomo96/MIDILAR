/**
 * @file LUT3D.h
 * @brief Base class for 3D Look-Up Tables (LUTs) used in dspFoundation.
 */

 #ifndef MIDILAR_LUT_3D_H
 #define MIDILAR_LUT_3D_H
 
 #include <stdlib.h>
 #include <math.h>
 #include <stdint.h>
 
 #include <MIDILAR_BuildSettings.h>
 
 namespace MIDILAR::dspFoundation::LUT {
 
     /**
      * @class LUT3D
      * @brief A template-based 3D Look-Up Table for function approximation with three variables.
      * 
      * @tparam OUTPUT_TYPE The data type stored in the LUT.
      */
     template<typename OUTPUT_TYPE>
     class LUT3D {
     public:
         using Type = LUT3D<OUTPUT_TYPE>;
 
     private:
         size_t _width;    ///< X dimension (e.g., phase)
         size_t _height;   ///< Y dimension (e.g., morph)
         size_t _depth;    ///< Z dimension (e.g., exponentiation)
         OUTPUT_TYPE* _buffer; ///< Pointer to flattened 3D buffer
 
         bool _ResizeBuffer(size_t width, size_t height, size_t depth);
 
     protected:
         /**
          * @brief Pure virtual function to evaluate and fill the LUT.
          */
         virtual void Eval() = 0;
 
         OUTPUT_TYPE* GetBuffer();
         const OUTPUT_TYPE* GetBuffer() const;
 
         void SetData(size_t x, size_t y, size_t z, OUTPUT_TYPE value);
         void ClearBuffer();
         void SetRawData(OUTPUT_TYPE* data, size_t width, size_t height, size_t depth);
 
     public:
        LUT3D();
        virtual ~LUT3D();
 
        bool SetBufferSize(size_t width, size_t height, size_t depth);
        size_t Width() const;
        size_t Height() const;
        size_t Depth() const;
 
        virtual OUTPUT_TYPE GetValue(size_t x, size_t y, size_t z) const;
        OUTPUT_TYPE GetValue(size_t x, size_t y) const;
        OUTPUT_TYPE GetValue(size_t x) const ;
        
        LUT3D(LUT3D&& other) noexcept;
        LUT3D& operator=(LUT3D&& other) noexcept;
     };
 
 } // namespace MIDILAR::dspFoundation::LUT
 
 #include "LUT3D.tpp"
 
 #endif // MIDILAR_LUT_3D_H
 