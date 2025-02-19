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
     * @tparam INPUT_TYPE The range type used for indexing the LUT (default is float).
     * @tparam OUTPUT_TYPE The data type stored in the LUT.
     */
        template<typename INPUT_TYPE = uint8_t, typename OUTPUT_TYPE = uint8_t>
        class LUT1D{
    
            private:
                size_t _bufferSize;         ///< The number of elements in the LUT.
                
                INPUT_TYPE _InputRangeMin;  ///< The minimum input range of the LUT. (x axis)
                INPUT_TYPE _InputRangeMax;  ///< The maximum input range of the LUT. (x axis)
                
                float _InputGain;
                float _InputOffset;
                
                enum InputPipeline : uint8_t {
                    Bypass        = 0b000,
                    Scaling       = 0b001,
                    Offsetting    = 0b010,
                    Interpolation = 0b100
                };

                InputPipeline _Pipeline;

                // Private Static Methods for Bitwise Operations
                static constexpr InputPipeline CombineFlags(InputPipeline a, InputPipeline b) {
                    return static_cast<InputPipeline>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
                }

                // Method to Check Flags Internally
                bool HasPipelineFlag(InputPipeline flag) const {
                    return (_Pipeline & flag) != 0;
                }


            protected:
                
                OUTPUT_TYPE* _buffer;  ///< Pointer to the allocated LUT buffer.    
            
            private:

                    void DetectMode();
                    void _RecalculateConstants();
                    bool _ResizeBuffer(size_t newSize);
            
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
                 * @brief Sets the LUT buffer size.
                 * 
                 * Resizes the buffer and triggers a recalculation of the LUT.
                 * 
                 * @param size The new size of the buffer.
                 * @return True if resizing was successful, false otherwise.
                 */
                    bool SetBufferSize(size_t size);
                //
                /////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Sets the LUT buffer size.
                 * 
                 * Resizes the buffer and triggers a recalculation of the LUT.
                 * 
                 * @param size The new size of the buffer.
                 * @return True if resizing was successful, false otherwise.
                 */
                    size_t BufferSize() const;
                //
                /////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Clears the LUT buffer but retains allocated memory.
                 * 
                 * Sets all elements in the buffer to the default value of `OUTPUT_TYPE`.
                 */
                    void ClearBuffer();
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
                 * @brief Destructor for LUT1D.
                 * Frees the allocated buffer memory.
                 */
                    virtual ~LUT1D();
                //
                /////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Sets the input range for the LUT.
                 * 
                 * The range is used for interpolation during lookup operations.
                 * 
                 * @param minVal The minimum input range.
                 * @param maxVal The maximum input range.
                 */
                    void SetInputRange(INPUT_TYPE minVal, INPUT_TYPE maxVal);
                //
                /////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Gets the minimum input value of the LUT.
                 * @return The minimum range value.
                 */
                    inline INPUT_TYPE InputMin() const;
                //
                /////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Gets the maximum input value of the LUT.
                 * @return The maximum range value.
                 */
                    inline INPUT_TYPE InputMax() const;
                //
                /////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Retrieves an interpolated value from the LUT.
                 * 
                 * Uses linear interpolation between LUT points based on the input value.
                 * 
                 * @param inputValue The input value to lookup.
                 * @param InterpolateOutput Whether to use interpolation.
                 * @return The interpolated LUT value.
                 */
                    OUTPUT_TYPE GetValue(INPUT_TYPE inputValue, bool Interpolate = 0, bool Wrapping = 0) const;
                //
                /////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Retrieves an interpolated value from the LUT.
                 * 
                 * Uses linear interpolation between LUT points based on the input value.
                 * 
                 * @param inputValue The input value to lookup.
                 * @param InterpolateOutput Whether to use interpolation.
                 * @return The interpolated LUT value.
                 */
                    void SetRawData(OUTPUT_TYPE* Data, size_t buffer_size);
                //
                /////////////////////////////////////////////////////////////////////////
        };
    //
    /////////////////////////////////////////////////////////////////////////////////////

 } // namespace MIDILAR::dspFoundation::LUT
 
 #include "LUT1D.tpp"
 
 #endif // MIDILAR_LUT_1D_H
 