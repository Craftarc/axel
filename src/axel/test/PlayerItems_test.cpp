#include "axel/PlayerItems.h"

#include <fstream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "config/poe.h"
#include "parse/util.h"
#include "util/MockHttpSender.h"

using namespace ::testing;

namespace {
    std::string s{ "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" };

}  // namespace

namespace axel {
    class PlayerItems_test : public ::Test {
        protected:
            PlayerItems_test() :
                mock_http_sender{ std::make_shared<util::MockHttpSender>() },
                player_items{ s, mock_http_sender } {
                std::fstream list_stashes_file{ "../test/poe/list-stashes.json" };
                list_stashes_file_string = parse::read_file_into_string(list_stashes_file);

                std::fstream bad_list_stashes_file{ "../test/poe/bad-list-stashes.json" };
                bad_list_stashes_file_string = parse::read_file_into_string(bad_list_stashes_file);

                std::fstream get_stash_file{ "../test/poe/get-stash.json" };
                get_stash_file_string = parse::read_file_into_string(get_stash_file);

                std::fstream bad_get_stash_file{ "../test/poe/bad-get-stash.json" };
                bad_get_stash_file_string = parse::read_file_into_string(bad_get_stash_file);
            }

            std::shared_ptr<util::MockHttpSender> mock_http_sender;
            PlayerItems player_items;
            std::string list_stashes_file_string;
            std::string bad_list_stashes_file_string;
            std::string get_stash_file_string;
            std::string bad_get_stash_file_string;
    };

    TEST_F(PlayerItems_test, PlayerItems__valid_access_token) {
        EXPECT_NO_THROW(PlayerItems("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));  // 43-characters
    }

    TEST_F(PlayerItems_test, PlayerItems__invalid_access_token) {
        EXPECT_THROW(PlayerItems("invalid_access_token"), std::runtime_error);
    }

    TEST_F(PlayerItems_test, get_update__non_empty_items_table) {
        auto request_responder = [&](auto request) {
            if (request.target() == config::poe::paths::list_stashes) {  // Path used by list_stashes for HTTP request
                return list_stashes_file_string;
            } else if (request.target() == config::poe::paths::get_stash + "020a8ddeef") {  // Path used by get_stash
                return get_stash_file_string;
            }
        };
        ON_CALL(*mock_http_sender, send_http_request(_)).WillByDefault(request_responder);

        // Make the method call
        auto items_table(player_items.get_update());
        EXPECT_FALSE(items_table.empty());
    }

    TEST_F(PlayerItems_test, get_update__bad_list_stashes) {
        auto request_responder = [&](auto request) {
            if (request.target() == config::poe::paths::list_stashes) {  // Path used by list_stashes for HTTP request
                return bad_list_stashes_file_string;
            } else if (request.target() == config::poe::paths::get_stash + "020a8ddeef") {  // Path used by get_stash
                return get_stash_file_string;
            }
        };

        ON_CALL(*mock_http_sender, send_http_request(_)).WillByDefault(request_responder);

        EXPECT_THROW(player_items.get_update(), std::runtime_error);
    }

    TEST_F(PlayerItems_test, get_update__bad_get_stash) {
        auto request_responder = [&](auto request) {
            if (request.target() == config::poe::paths::list_stashes) {  // Path used by list_stashes for HTTP request
                return list_stashes_file_string;
            } else if (request.target() == config::poe::paths::get_stash + "020a8ddeef") {  // Path used by get_stash
                return bad_get_stash_file_string;
            }
        };

        ON_CALL(*mock_http_sender, send_http_request(_)).WillByDefault(request_responder);

        EXPECT_THROW(player_items.get_update(), std::runtime_error);
    }
}  // namespace axel