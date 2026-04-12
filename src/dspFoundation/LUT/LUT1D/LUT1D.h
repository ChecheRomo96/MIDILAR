/**
 * @file LUT1D.h
 * @brief Base class for Look-Up Tables (LUTs) used in dspFoundation.
 */

 #ifndef MIDILAR_LUT_1D_H
 #define MIDILAR_LUT_1D_H
 
 #include <stdlib.h> // For malloc/free
 #include <math.h>
 #include <stdint.h>

 #include <System/BuildSettings.h>
 
 namespace MIDILAR::dspFoundation::LUT {
    
    /////////////////////////////////////////////////////////////////////////////////////
    /**
     * @class LUT1D
     * @brief A template-based Look-Up Table (LUT) for efficient function approximation.
     * 
     * This class provides a base implementation for LUT-based interpolation.
     * Derived classes should override the `Eval()` method to populate the LUT.
     *
     * @tparam OUTPUT_TYPE The data type stored in the LUT.
     */
        template<typename OUTPUT_TYPE>
        class LUT1D{
            public:
                using Type = LUT1D<OUTPUT_TYPE>;

            private:

                    bool _ResizeBuffer(size_t newSize);
                    size_t _bufferSize;         ///< The number of elements in the LUT.
                    OUTPUT_TYPE* _buffer;  ///< Pointer to the allocated LUT buffer. 
            
            protected:
            
                /////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Pure virtual function for LUT evaluation.
                 * 
                 * Derived classes must override this method to populate the LUT with appropriate values.
                 */
                    virtual void Eval() = 0;
                //
                /////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Returns a raw pointer to the LUT buffer.
                 * Use with care. Intended for derived classes or internal use.
                 */
                    OUTPUT_TYPE* GetBuffer();
                //
                /////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Returns a const pointer to the internal LUT buffer.
                 */
                    const OUTPUT_TYPE* GetBuffer() const;
                //
                /////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Sets the LUT buffer size.
                 * 
                 * Allocates or resizes the buffer and triggers re-evaluation
                 * 
                 * @param size The new number of LUT entries.
                 * @return True if successful, false otherwise.
                 */
                    bool SetBufferSize(size_t size);
                //
                /////////////////////////////////////////////////////////////////////////   
                /**
                 * @brief Returns the current buffer size.
                 * 
                 * @return Number of entries in the LUT.
                 */
                    size_t BufferSize() const;
                //
                /////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Sets a specific data value at a given bin index.
                 * 
                 * @param index The bin index to modify.
                 * @param Data  The value to set.
                 */
                    void SetData(size_t index, OUTPUT_TYPE Data);
                //
                /////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Clears all LUT values without deallocating memory.
                 * 
                 * Sets every entry to zero.
                 */
                    void ClearBuffer();
                //
                /////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Sets the LUT data from an external source.
                 * 
                 * Transfers external buffer data into the LUT.
                 * Caller is responsible for memory safety.
                 * 
                 * @param Data Pointer to external buffer.
                 * @param buffer_size Number of elements in the external buffer.
                 */
                    void SetRawData(OUTPUT_TYPE* Data, size_t buffer_size);
                //
                /////////////////////////////////////////////////////////////////////////
            
            public:
            
                /////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Constructor for LUT1D.
                 * Initializes the LUT with no allocated buffer.
                 */
                    LUT1D();
                //
                /////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Move constructor.
                 * 
                 * Transfers ownership of the LUT buffer from another instance.
                 * 
                 * @param other The LUT1D instance to move from.
                 */
                    LUT1D(LUT1D&& other) noexcept;
                //
                /////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Move assignment operator.
                 * 
                 * Transfers ownership of the LUT buffer from another instance.
                 * 
                 * @param other The LUT1D instance to move from.
                 * @return Reference to this instance.
                 */
                    LUT1D& operator=(LUT1D&& other) noexcept;
                //
                /////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Destructor for LUT1D.
                 * Frees the allocated buffer memory.
                 */
                    virtual ~LUT1D();
                //
                /////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Retrieves an interpolated value from the LUT.
                 * 
                 * Uses linear interpolation between LUT points based on the input value.
                 * 
                 * @param inputValue The input value to lookup.
                 * @return The interpolated LUT value.
                 */
                    virtual OUTPUT_TYPE GetValue(size_t bin) const;
                //
                /////////////////////////////////////////////////////////////////////////
        };
    //
    /////////////////////////////////////////////////////////////////////////////////////

 } // namespace MIDILAR::dspFoundation::LUT
 
 #include "LUT1D.tpp"
 
 #endif // MIDILAR_LUT_1D_H
 