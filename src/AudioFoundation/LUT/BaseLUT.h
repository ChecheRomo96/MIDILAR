/**
 * @file MIDILAR_LookUpTable.h
 * @brief Base class for Look-Up Tables (LUTs) used in AudioFoundation.
 */

#ifndef MIDILAR_LOOK_UP_TABLE_H
#define MIDILAR_LOOK_UP_TABLE_H

    #include <System/BuildSettings.h>
    #include <stdlib.h> // For malloc/free
    #include <math.h>

    namespace MIDILAR::AudioFoundation::LUT {

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @class BaseLUT
         * @brief A template-based Look-Up Table (LUT) for efficient function approximation.
         * 
         * This class provides a base implementation for LUT-based interpolation.
         * Derived classes should override the `Eval()` method to populate the LUT.
         *
         * @tparam LUT_TYPE The data type stored in the LUT.
         * @tparam LUT_RANGE_TYPE The range type used for indexing the LUT (default is float).
         */
            template<typename LUT_TYPE, typename LUT_RANGE_TYPE = float, typename LUT_INPUT_TYPE = float>
            class BaseLUT {

                private:

                    size_t _bufferSize;             ///< The number of elements in the LUT.
                    float _InputRangeMin;  ///< The minimum input range of the LUT. (x axis)
                    float _InputRangeMax;  ///< The maximum input range of the LUT. (x axis)

                    bool _InterpolateOutput;

                protected:

                    LUT_TYPE* _buffer;  ///< Pointer to the allocated LUT buffer.

                    ///////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Gets the size of the LUT buffer.
                     * @return The number of elements in the LUT.
                     */
                        inline size_t Size() const { return _bufferSize; }
                    //
                    ///////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Gets the minimum input value of the LUT.
                     * @return The minimum range value.
                     */
                        inline float InputMin() const { return _InputRangeMin; }
                    //
                    ///////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Gets the maximum input value of the LUT.
                     * @return The maximum range value.
                     */
                        inline float InputMax() const { return _InputRangeMax; }
                    //
                    ///////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Resizes the LUT buffer.
                     * 
                     * Allocates or reallocates memory for the LUT while preserving existing values.
                     * 
                     * @param newSize The new buffer size.
                     * @return True if resizing was successful, false otherwise.
                     */
                        bool _ResizeBuffer(size_t newSize) {
                            if (newSize == 0) return false; // Prevent zero allocation

                            LUT_TYPE* newBuffer = static_cast<LUT_TYPE*>(malloc(newSize * sizeof(LUT_TYPE)));
                            if (!newBuffer) return false; // Allocation failed

                            if (_buffer) {
                                free(_buffer);
                            }

                            _buffer = newBuffer;
                            _bufferSize = newSize;
                            return true;
                        }
                    //
                    ///////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Pure virtual function for LUT evaluation.
                     * 
                     * Derived classes must override this method to populate the LUT with appropriate values.
                     */
                        virtual void Eval() = 0;
                    //
                    ///////////////////////////////////////////////////////////////////////////////////

                public:

                    ///////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Constructor for BaseLUT.
                     * Initializes the LUT with no allocated buffer.
                     */
                        BaseLUT() {
                            _buffer = nullptr;
                            _bufferSize = 0;
                            _InputRangeMin = 0.0f;
                            _InputRangeMax = 1.0f;
                            _InterpolateOutput = false;
                        }
                    //
                    ///////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Destructor for BaseLUT.
                     * Frees the allocated buffer memory.
                     */
                        virtual ~BaseLUT() {
                            if (_buffer) {
                                free(_buffer);
                            }
                        }
                    //
                    ///////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Sets the LUT buffer size.
                     * 
                     * Resizes the buffer and triggers a recalculation of the LUT.
                     * 
                     * @param size The new size of the buffer.
                     * @return True if resizing was successful, false otherwise.
                     */
                        bool SetBufferSize(size_t size) {
                            if (size == _bufferSize) return true;  // Skip if no change
                            if (_ResizeBuffer(size)) {
                                Eval(); // Ensure buffer is recalculated
                                return true;
                            }
                            return false;
                        }
                    //
                    ///////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Sets the input range for the LUT.
                     * 
                     * The range is used for interpolation during lookup operations.
                     * 
                     * @param minVal The minimum input range.
                     * @param maxVal The maximum input range.
                     */
                        void SetInputRange(float minVal, float maxVal) {
                            if (minVal == _InputRangeMin && maxVal == _InputRangeMax) return;  // Skip if unchanged
                            _InputRangeMin = minVal;
                            _InputRangeMax = maxVal;
                            Eval(); // Ensure buffer is recalculated
                        }
                    //
                    ///////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Retrieves an interpolated value from the LUT.
                     * 
                     * Uses linear interpolation between LUT points based on the input value.
                     * 
                     * @param inputValue The input value to lookup.
                     * @return The interpolated LUT value.
                     */
                        LUT_TYPE GetValue(LUT_RANGE_TYPE inputValue) const {
                            if (!_buffer || _bufferSize == 0) {
                                return LUT_TYPE(); // Return default value if buffer is uninitialized
                            }

                            // Handle case where input range is nearly identical (avoid division by zero)
                            if (abs(_InputRangeMax - _InputRangeMin) < 1e-6f) {
                                return _buffer[0]; // Prevents invalid calculations
                            }

                            // Normalize input value for indexing into the LUT
                            float normalizedPosition = (inputValue - _InputRangeMin) / (_InputRangeMax - _InputRangeMin);

                            // Proper wrapping without using fmod()
                            normalizedPosition -= floor(normalizedPosition);  // Ensure it's in [0,1)
                            if (normalizedPosition < 0.0f) normalizedPosition += 1.0f;   // Ensure positive wrap-around

                            // Compute indices for interpolation
                            float indexFloat = normalizedPosition * (_bufferSize); // Scale correctly
                            size_t lowerIndex = static_cast<size_t>(floor(indexFloat));
                            size_t upperIndex = (lowerIndex + 1) % _bufferSize;  // Circular wrap-around

                            // If interpolation is disabled, return the nearest LUT value immediately
                            if (!_InterpolateOutput) {
                                return _buffer[lowerIndex];
                            }

                            // Compute interpolation factor
                            float interpolationFactor = indexFloat - lowerIndex;

                            // Perform linear interpolation
                            LUT_TYPE lowerValue = _buffer[lowerIndex];
                            LUT_TYPE upperValue = _buffer[upperIndex];

                            return lowerValue + (upperValue - lowerValue) * interpolationFactor;
                        }

                    //
                    ///////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Clears the LUT buffer but retains allocated memory.
                     * 
                     * Sets all elements in the buffer to the default value of `LUT_TYPE`.
                     */
                        void ClearBuffer() {
                            if (_buffer) {
                                for (size_t i = 0; i < _bufferSize; i++) {
                                    _buffer[i] = LUT_TYPE();
                                }
                            }
                        }
                    //
                    ///////////////////////////////////////////////////////////////////////////////////
                    //

                        void OutputInterpolation_Enable(){
                            _InterpolateOutput = true;
                        }

                        void OutputInterpolation_Disable(){
                            _InterpolateOutput = false;
                        }

                        void SetOutputInterpolation(bool mode){
                            _InterpolateOutput = mode;
                        }

                        bool OutputInterpolationStatus(){
                            return _InterpolateOutput;
                        }
            };
        //
        ///////////////////////////////////////////////////////////////////////////////////////////////
    }

#endif // MIDILAR_LOOK_UP_TABLE_H
