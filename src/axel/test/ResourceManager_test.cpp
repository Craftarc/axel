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
                mock_http_sender_handle{ new util::MockHttpSender },
                mock_http_sender{ mock_http_sender_handle },
                resource_manager{ "access_token", mock_http_sender } {};

            util::MockHttpSender* mock_http_sender_handle;
            std::shared_ptr<util::MockHttpSender> mock_http_sender;
            axel::ResourceManager resource_manager;

            // Dummy items
            axel::Item item_0{ 0, "item_0", 0, 0 };
            axel::Item item_1{ 1, "item_1", 1, 1 };
            axel::Item item_2{ 2, "item_2", 2, 2 };
            axel::Item item_3{ 3, "item_3", 3, 3 };
    };

    /* TEST_F(ResourceManager_test, get_update__valid_json) { */
    /*     EXPECT_CALL(*mock_http_sender_handle,  */
    /* } */

}  // namespace axel
