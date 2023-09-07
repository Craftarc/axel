#include "util/HttpSender.h"

#include <gtest/gtest.h>

#include "util/http.h"

using namespace ::testing;

class HttpSender_test : public Test {
	protected:
		util::HttpSender http_sender{};
};

TEST_F(HttpSender_test, send_http_request__accepts_empty_path) {
	auto request = util::make_http_request("GET", "", { { "host", "www.google.com" } }, "");

	// In Google Test, we use EXPECT_NO_THROW to assert that a statement doesn't throw an exception
	EXPECT_NO_THROW(http_sender.send_http_request(request, 15));
}

TEST_F(HttpSender_test, send_http_request__accepts_root_path) {
	auto request = util::make_http_request("GET", "/", { { "host", "www.google.com" } }, "");

	EXPECT_NO_THROW(http_sender.send_http_request(request, 15));
}

TEST_F(HttpSender_test, send_http_request__reject_protocol_in_host) {
	auto request = util::make_http_request("GET", "/", { { "host", "https://www.google.com" } }, "");

	EXPECT_THROW(http_sender.send_http_request(request, 15), std::runtime_error);
}
