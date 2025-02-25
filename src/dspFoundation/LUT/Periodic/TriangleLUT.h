#ifndef MIDILAR_TRIANGLE_LUT_H
#define MIDILAR_TRIANGLE_LUT_H

    #include <System/BuildSettings.h>
    #include <math.h>
    #include <dspFoundation/LUT/LUT1D/LUT1D.h>

    #ifndef TWO_PI
        #define TWO_PI 6.28318530718f
    #endif

    namespace MIDILAR::dspFoundation::LUT::Periodic {

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @class TriangleLUT
         * @brief A sawtooth wave lookup table.
         * 
         * This class precomputes sawtooth wave values for fast lookup and interpolation.
         * 
         * @tparam INPUT_TYPE  The data type stored in the LUT (default: float).
         * @tparam OUTPUT_TYPE The range type for indexing the LUT (default: float).
         */
        template<typename INPUT_TYPE, typename OUTPUT_TYPE>
        class TriangleLUT : public LUT::LUT1D<INPUT_TYPE, OUTPUT_TYPE> {
        private:
            float _phase_offset;  // Stored in Radians
            float _amplitude;
            float _offset;

        protected:
            /**
             * @brief Evaluates and populates the LUT with sawtooth wave values.
             */
            void Eval() override {
                size_t bufferSize = this->BufferSize();
                if (!this->_buffer || bufferSize == 0) return;
                TriangleGenerator(this->_buffer, bufferSize, _amplitude, _offset, _phase_offset);
            }

        public:
            /**
             * @brief Constructor for TriangleLUT.
             */
            TriangleLUT()
                : LUT1D<INPUT_TYPE, OUTPUT_TYPE>(),
                _phase_offset(0.0f),
                _amplitude(1.0f),
                _offset(0.0f) {
                this->SetInputRange(0.0f, TWO_PI); // Set LUT range to 0 - 2π
                Eval();
            }
                
            bool SetBufferSize(size_t size){
                return LUT::LUT1D<INPUT_TYPE, OUTPUT_TYPE>::SetBufferSize(size);
            }

            void SetPhaseOffsetNormWavelengths(float Offset, bool Update = true) {
                float newOffset = fmodf(Offset * TWO_PI, TWO_PI);
                if (newOffset < 0.0f) newOffset += TWO_PI;
                if (_phase_offset == newOffset) return;
                _phase_offset = newOffset;
                if (Update) Eval();
            }

            void SetPhaseOffsetRads(float Offset, bool Update = true) {
                float newOffset = fmodf(Offset, TWO_PI);
                if (newOffset < 0.0f) newOffset += TWO_PI;
                if (_phase_offset == newOffset) return;
                _phase_offset = newOffset;
                if (Update) Eval();
            }

            void SetPhaseOffsetDeg(float Offset, bool Update = true) {
                float newOffset = fmodf(Offset * (TWO_PI / 360.0f), TWO_PI);
                if (newOffset < 0.0f) newOffset += TWO_PI;
                if (_phase_offset == newOffset) return;
                _phase_offset = newOffset;
                if (Update) Eval();
            }

            void SetAmplitude(float Amp, bool Update = true) {
                if (_amplitude == Amp) return;
                _amplitude = Amp;
                if (Update) Eval();
            }

            void SetDCOffset(float Offset, bool Update = true) {
                if (_offset == Offset) return;
                _offset = Offset;
                if (Update) Eval();
            }
        };

    } // namespace MIDILAR::dspFoundation::LUT::Periodic

#endif // MIDILAR_TRIANGLE_LUT_H
