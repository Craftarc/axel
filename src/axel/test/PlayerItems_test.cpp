#include "axel/PlayerItems.h"

#include <fstream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "config/poe.h"
#include "parse/util.h"
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

    TEST_F(PlayerItems_test, get_update__empty_json_response) {
        NiceMock<util::MockHttpSender>* mock_http_sender_handle{
            mock_http_sender.get()
        };
        std::string test_json{ "{}" };
        EXPECT_CALL(*mock_http_sender_handle, send_http_request(_))
        .Times(1)
        .WillOnce(Return(test_json));

        // Should throw something because nothing can be found in the empty json
        EXPECT_THROW(player_items.get_update(""), std::runtime_error);
    }
}  // namespace axel
