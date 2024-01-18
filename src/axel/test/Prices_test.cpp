#include "axel/Prices.h"

#include <boost/json.hpp>
#include <gtest/gtest.h>
#include <parse/util.h>

#include "axel/Config.h"
#include "util/MockDatabase.h"
#include "util/MockHttpSender.h"

namespace {
    bool is_kebab_case(const std::string& str) {
        if (str.empty() || str.front() == '-' || str.back() == '-') {
            return false;
        }

        if (str.find("--") != std::string::npos) {
            return false;
        }

        return std::all_of(str.begin(), str.end(), [](char ch) {
            return std::islower(ch) || std::isdigit(ch) || ch == '-';
        });
    }

    bool is_valid_unit_price(double unit_price) {
        return (unit_price == -1 || unit_price >= 0);
    }
}  // namespace

namespace axel {
    using namespace ::testing;
    namespace json = boost::json;

    class Prices_test : public Test {
        protected:
            Prices_test() :
                prices_{ CONFIG("api.ninja.leagues.standard"),
                         CONFIG("api.ninja.leagues.softcore"),
                         mock_http_sender_,
                         std::move(mock_database_) },

                // HttpSender
                mock_http_sender_handle_{
                    new NiceMock<util::MockHttpSender>()
                },
                mock_http_sender_{ std::shared_ptr<util::MockHttpSender>(
                mock_http_sender_handle_) },

                // Database
                mock_database_handle_{ new NiceMock<util::MockDatabase>() },
                mock_database_{
                    std::unique_ptr<util::MockDatabase>(mock_database_handle_)
                } {}

            util::MockHttpSender* mock_http_sender_handle_;
            std::shared_ptr<util::MockHttpSender> mock_http_sender_;
            util::MockDatabase* mock_database_handle_;
            std::unique_ptr<util::MockDatabase> mock_database_;
            Prices prices_;

            std::string path = CONFIG("api.ninja.endpoint.currency");
    };

    TEST_F(Prices_test, get_prices__calls_http_sender) {
        EXPECT_CALL(*mock_http_sender_handle_, send_http_request(_, _));
        EXPECT_NO_THROW(prices_.get_prices(path));
    }

    TEST_F(Prices_test, get_prices__bad_response_empty_table) {
        json::value bad_response{ { "key", "value" } };  // Missing 'lines' item
        EXPECT_CALL(*mock_http_sender_handle_, send_http_request(_, _))
        .WillOnce(Return(json::serialize(bad_response)));
        std::unordered_map result = prices_.get_prices(path);
        EXPECT_EQ(true, result.empty());
    }

    TEST_F(Prices_test, get_prices__good_response_ok) {
        std::fstream file("../sample/poe-ninja/currency.json");
        std::string good_response = parse::read_file_into_string(file);

        EXPECT_CALL(*mock_http_sender_handle_, send_http_request(_, _))
        .WillOnce(Return(good_response));
        std::unordered_map result = prices_.get_prices(path);
        EXPECT_EQ(false, result.empty());

        for (auto& pair : result) {
            EXPECT_TRUE(is_kebab_case(pair.first));
            EXPECT_TRUE(is_valid_unit_price(pair.second));
        }
    }
}  // namespace axel
