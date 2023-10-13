#include "axel/ResourceManager.h"

#include <fstream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "parse/util.h"
#include "util/MockHttpSender.h"
using namespace ::testing;

namespace axel {
    class ResourceManager_test : public Test {
        protected:
            ResourceManager_test() :
                mock_http_sender{ new util::MockHttpSender },
                shared_mock_http_sender{ mock_http_sender },
                resource_manager{ "access_token", shared_mock_http_sender } {};

            util::MockHttpSender* mock_http_sender;
            std::shared_ptr<util::MockHttpSender> shared_mock_http_sender;
            axel::ResourceManager resource_manager;
    };

    // Get update shouldn't return an empty array (unless the player has literally zero items)
    TEST_F(ResourceManager_test, get_update__no_empty_items) {
        auto result{ resource_manager.get_update() };
        EXPECT_FALSE(result.empty());
    }

    TEST_F(ResourceManager_test, get_update__succeed) {
        EXPECT_NO_FATAL_FAILURE(resource_manager.get_update());
    }
}  // namespace axel
