#include <gtest/gtest.h>
#include <MIDILAR_dspFoundation.h>

namespace {

    MIDILAR::dspFoundation::LUT::Periodic::SineLUT<float>   floatLUT;
    MIDILAR::dspFoundation::LUT::Periodic::SineLUT<uint8_t> uintLUT;
    MIDILAR::dspFoundation::LUT::Periodic::SineLUT<int16_t> intLUT;

    class TestSineLUT : public ::testing::Test {
        protected:
            void SetUp() override {
                floatLUT.SetBufferSize(0);
            }
    
            void TearDown() override {
                // Cleanup (automatically handled by destructor)
            }
    };

    TEST(TestSineLUT, Integer){

    }
}