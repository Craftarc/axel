#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "axel/HandlerManager.h"

using namespace ::testing;
namespace {
    class HandlerManager_test : public Test {
    protected:
        axel::HandlerManager test_handler_manager{"test"};
    };
}