#include "axel/PlayerItems.h"

#include <fstream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "config/poe.h"
#include "parse/util.h"
#include "util/MockHttpSender.h"

using namespace ::testing;

namespace {}

namespace axel {
    class PlayerItems_test : public ::Test {
        protected:
            PlayerItems_test() :
                mock_http_sender{ std::make_shared<util::MockHttpSender>() },
                player_items{ "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", mock_http_sender } {}

            std::shared_ptr<util::MockHttpSender> mock_http_sender;
            PlayerItems player_items;
    };

    TEST_F(PlayerItems_test, PlayerItems__valid_access_token) {
        EXPECT_NO_THROW(PlayerItems("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));  // 43-characters
    }

    TEST_F(PlayerItems_test, PlayerItems__invalid_access_token) {
        EXPECT_THROW(PlayerItems("invalid_access_token"), std::runtime_error);
    }

    TEST_F(PlayerItems_test, get_update__successful) {
        std::fstream list_stashes_file{ "../test/poe/list-stashes.json" };
        std::string list_stashes_file_string{ parse::read_file_into_string(list_stashes_file) };

        std::fstream get_stash_file{ "../test/poe/get-stash.json" };
        std::string get_stashes_file_string{ parse::read_file_into_string(get_stash_file) };

        auto request_responder = [&](auto request) {
            if (request.target() == config::poe::paths::list_stashes) {  // Path used by list_stashes for HTTP request
                std::cout << "list" << std::endl;
                return list_stashes_file_string;
            } else if (request.target() == config::poe::paths::get_stash + "020a8ddeef") {  // Path used by get_stash
                std::cout << "get" << std::endl;
                return get_stashes_file_string;
            }
        };
        ON_CALL(*mock_http_sender, send_http_request(_)).WillByDefault(request_responder);

        // Make the method call
        player_items.get_update();
    }

}  // namespace axel