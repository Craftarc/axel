#include <gtest/gtest.h>
#include "axel/TestHandler.h"

using namespace ::testing;

class TestHandler_test : public Test {
    axel::TestHandler test_handler{};
};

TEST_F(TestHandler_test, placeholder) {
    EXPECT_EQ(1, 1);
}