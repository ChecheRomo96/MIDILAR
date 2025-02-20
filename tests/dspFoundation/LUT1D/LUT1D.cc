#include <gtest/gtest.h>
#include <MIDILAR_dspFoundation.h>

using namespace MIDILAR::dspFoundation::LUT;

namespace{
    template<typename OUTPUT_TYPE>
    class TestLUT : public LUT1D< OUTPUT_TYPE> {
        protected:

            
            void Eval() override {
                if (LUT1D<OUTPUT_TYPE>::BufferSize() == 0) return;
                
                // Example: Identity mapping LUT
                for (size_t i = 0; i < BufferSize(); i++) {
                    this->SetData(i, static_cast<OUTPUT_TYPE>(i));
                }
            }
        
        public:
            uint8_t AdditionalValue;
            /**
             * @brief Exposes the protected SetBufferSize method for testing.
             */
            bool SetBufferSize(size_t size) {
                return LUT1D<OUTPUT_TYPE>::SetBufferSize(size);
            }
        
            /**
             * @brief Exposes the protected BufferSize method for testing.
             */
            size_t BufferSize() const {
                return LUT1D<OUTPUT_TYPE>::BufferSize();
            }
            
            TestLUT& operator=(TestLUT&& other) noexcept {
                if (this != &other) {
                    LUT1D<OUTPUT_TYPE>::operator=(std::move(other));
                    // Move Derived class specific elements
                    AdditionalValue = other.AdditionalValue;
                    other.AdditionalValue = 0;
                }
                return *this;
            }
            

            void SetRawData(OUTPUT_TYPE* Data, size_t buffer_size){
                LUT1D<OUTPUT_TYPE>::SetRawData(Data,buffer_size);
            }

            void ClearBuffer(){
                LUT1D<OUTPUT_TYPE>::ClearBuffer();
            }


    };

// Define a simple LUT instance for testing

    TestLUT<float>        lutA;
    using lutA_Type = TestLUT<float>::Type;

    TestLUT<uint8_t>      lutB;
    using lutB_Type = TestLUT<uint8_t>::Type;

    class LUT1DTest : public ::testing::Test {
    protected:

        void SetUp() override {
            lutA.ClearBuffer();
            lutB.ClearBuffer();
        }

        void TearDown() override {
            // Cleanup (automatically handled by destructor)
        }
    };


    TEST(LUT1DTest, BufferSize){

        lutA.SetBufferSize(10);
        for (size_t i = 0; i < lutA.BufferSize(); i++) {
            // Get actual LUT output
            float result = lutA.GetValue(i);
            // Validate against expected truncated output
            EXPECT_NEAR(static_cast<float>(i), result, 1e-5);
        }

        lutB.SetBufferSize(10);
        for (size_t i = 0; i < lutB.BufferSize(); i++) {
            // Get actual LUT output
            uint8_t result = lutB.GetValue(i);
            // Validate against expected truncated output
            EXPECT_EQ(i, result);
        }
        
        lutA.ClearBuffer();
        for (size_t i = 0; i < lutA.BufferSize(); i++) {
            // Get actual LUT output
            float result = lutA.GetValue(i);
            // Validate against expected truncated output
            EXPECT_NEAR(float(), result, 1e-5);
        }

        lutB.ClearBuffer();
        for (size_t i = 0; i < lutB.BufferSize(); i++) {
            // Get actual LUT output
            uint8_t result = lutB.GetValue(i);
            // Validate against expected truncated output
            EXPECT_EQ(uint8_t(), result);
        }
    }


    TEST(LUT1DTest, SetRawData){

        uint8_t RawData[5] = {
            0,1,2,3,4
        };
        
        float RawDataf[5] = {
            0.0f,1.0f,2.0f,3.0f,4.0f
        };

        lutA.SetRawData(RawDataf,5);
        EXPECT_EQ(lutA.BufferSize(), 5);

        for (size_t i = 0; i < lutA.BufferSize(); i++) {
            // Get actual LUT output
            float result = lutA.GetValue(i);
            // Validate against expected truncated output
            EXPECT_NEAR(static_cast<float>(RawDataf[i]), result, 1e-5);
        }

        lutB.SetRawData(RawData,5);
        EXPECT_EQ(lutB.BufferSize(), 5);

        for (size_t i = 0; i < lutB.BufferSize(); i++) {
            // Get actual LUT output
            uint8_t result = lutB.GetValue(i);
            // Validate against expected truncated output
            EXPECT_EQ(RawData[i], result);
        }
    }

    TEST(LUT1DTest, MoveConstructor){
        lutA.SetBufferSize(10);
        lutA.AdditionalValue = 5;

        EXPECT_EQ(lutA.BufferSize(),10);
        EXPECT_EQ(lutA.AdditionalValue,5);

        TestLUT<float> tmpA;
        tmpA = std::move(lutA);

        EXPECT_EQ(lutA.BufferSize(),0);
        EXPECT_EQ(lutA.AdditionalValue,0);

        EXPECT_EQ(tmpA.BufferSize(),10);
        EXPECT_EQ(tmpA.AdditionalValue,5);
    }

}