#include "axel/PlayerItems.h"

#include <fstream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "util/MockHttpSender.h"

using namespace ::testing;

namespace axel {
    class PlayerItems_test : public ::Test {
        protected:
            PlayerItems_test() :
                mock_http_sender{
                    std::make_shared<NiceMock<util::MockHttpSender>>()
                },
                player_items{ "access_token", mock_http_sender } {}

            std::shared_ptr<NiceMock<util::MockHttpSender>> mock_http_sender;
            axel::PlayerItems player_items;
    };

    TEST_F(PlayerItems_test, get_update__empty_json_response_empty_table) {
        NiceMock<util::MockHttpSender>* mock_http_sender_handle{
            mock_http_sender.get()
        };
        std::string test_json{ "{}" };
        EXPECT_CALL(*mock_http_sender_handle, send_http_request(_))
        .Times(1)
        .WillOnce(Return(test_json));

        auto table = player_items.get_update("");
        EXPECT_TRUE(table.empty());
    }
}  // namespace axel
