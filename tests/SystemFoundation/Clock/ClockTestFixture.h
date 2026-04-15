#ifndef MIDILAR_TEST_SYSTEMFOUNDATION_CLOCK_CLOCKTESTFIXTURE_H
#define MIDILAR_TEST_SYSTEMFOUNDATION_CLOCK_CLOCKTESTFIXTURE_H

#include <gtest/gtest.h>
#include "ClockMock.h"

namespace MIDILAR::Tests::SystemFoundation {

    class ClockTest : public testing::Test {
    protected:
        void SetUp() override {
            GClockMock.Reset();
        }

        void TearDown() override {
        }
    };

}

#endif
