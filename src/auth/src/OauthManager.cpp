#include "auth/OauthManager.h"

#include <iostream>
#include <utility>

#include <aws/core/client/ClientConfiguration.h>
#include <spdlog/spdlog.h>

#include "auth/PkceManager.h"
#include "auth/SessionManager.h"
#include "auth/StateHashManager.h"
#include "auth/TokenRequestManager.h"
#include "auth/interfaces/IStateHashManager.h"
#include "auth/interfaces/ITokenRequestManager.h"
#include "axel/Database.h"
#include "parse/util.h"
#include "src/auth/mock/auth/MockTokenRequestManager.h"
#include "util/HttpSender.h"
#include "util/path.h"

namespace {
	int MAX_AUTH_CODE_TIME = 30;   // Maximum time before authorisation code expires and can no longer be exchanged
	int MAX_AUTH_START_TIME = 30;  // Maximum time between first start_auth() and redirect response

	/// @brief Returns the Unix time in seconds
	int64_t get_time_now() {
		std::chrono::time_point time_now{ std::chrono::system_clock::now() };

		// Get duration member
		auto time_point_duration{ time_now.time_since_epoch() };

		// Cast to seconds
		auto duration_seconds{ std::chrono::duration_cast<std::chrono::seconds>(time_point_duration) };

		// Extract count member, the number of seconds
		return duration_seconds.count();
	}
}  // namespace

namespace auth {
	OauthManager::OauthManager(const std::string& auth_database, const std::string& app_database) :
		OauthManager{ std::make_unique<PkceManager>(),
		              std::make_unique<StateHashManager>(),
		              std::make_unique<TokenRequestManager>(),
		              std::make_unique<AuthCodeManager>(),
		              std::make_unique<SessionManager>(),
		              std::make_unique<axel::Database>(auth_database, config::axel::database::auth_attributes),
		              std::make_unique<axel::Database>(app_database, config::axel::database::app_attributes) } {}

	OauthManager::OauthManager(std::unique_ptr<IPkceManager> pkce_manager,
	                           std::unique_ptr<IStateHashManager> state_hash_manager,
	                           std::unique_ptr<ITokenRequestManager> token_request_manager,
	                           std::unique_ptr<IAuthCodeManager> auth_code_manager,
	                           std::unique_ptr<ISessionManager> session_manager,
	                           std::unique_ptr<axel::IDatabase> auth_database,
	                           std::unique_ptr<axel::IDatabase> app_database) :
		pkce_manager_(std::move(pkce_manager)),
		state_hash_manager_(std::move(state_hash_manager)),
		token_request_manager_(std::move(token_request_manager)),
		auth_code_manager_(std::move(auth_code_manager)),
		session_manager_(std::move(session_manager)),
		auth_database_(std::move(auth_database)),
		app_database_(std::move(app_database)),
		state_(State::INITIAL) {
		spdlog::info("OAuthManager: State set to INITIAL");
	};

	/// Responsible for calling functions and methods in sequence according to the Oauth specification.
	/// @return The invocation response payload to send back to the user.
	std::string auth::OauthManager::start_auth() {
		const int REDIRECT_STATUS_CODE = 302;

		int64_t time_to_live{ get_time_now() + MAX_AUTH_CODE_TIME };
		spdlog::debug("time to live set to '{}'", std::to_string(time_to_live));

		auto code_challenge = pkce_manager_->get_code_challenge();
		auto code_verifier = pkce_manager_->get_code_verifier();
		spdlog::info("OauthManager::start_auth - Code verifier and code challenge obtained");

		auto state_hash = state_hash_manager_->get_state_hash();
		spdlog::info("OauthManager::start_auth - State hash obtained");

		auto session_id = session_manager_->get_session_token();
		spdlog::info("OauthManager::start_auth - Session token obtained");

		if (auth_database_->put({ { "session_id", session_id },
		                          { "state_hash", state_hash },
		                          { "code_verifier", code_verifier },
		                          { "time_to_live", std::to_string(time_to_live) } })) {
			spdlog::info("Stored session id, code verifier and state hash in OAuth table");
		} else {
			spdlog::error(
			"OauthManager::start_auth - Failed to insert (session_id, state_hash, code_verifier) in OAuth Database");
			throw std::runtime_error("Failed to insert into OAuth Database");
		};

		std::string auth_url = auth_code_manager_->get_auth_url(code_challenge, state_hash);
		spdlog::debug("OauthManager: Authorisation URL obtained: {}", auth_url);

		// Construct invocation response
		auth_url = "https://" + auth_url;  // Location must be absolute path instead of only hostname
		spdlog::debug("OauthManager::start_auth - auth_url should be absolute path, is {}", auth_url);

		// Make invocation response
		std::unordered_map<std::string, std::string> headers = { { "Location", auth_url },
			                                                     { "Set-Cookie", session_id } };
		auto response = parse::make_invocation_response_payload(REDIRECT_STATUS_CODE, headers, "");

        spdlog::info("Response: {}", response);
		set_state(State::USER_REDIRECTED);
		spdlog::info("State set to USER_REDIRECTED");

		return response;
	}

	/// Processes the authorization response from the authorization server after the user has logged in and approved
	/// the required scopes.
	/// @param query_string The query_string string containing the query parameters "state" and "code", returned from
	/// the authorization server after a successful login by the user.
	/// @param session_id The session_id identifying the authentication request.
	/// @return The session token for tracking this user session.
	std::string auth::OauthManager::receive_auth(const std::string& query_string, const std::string& session_id) {
		int64_t time_to_live{ get_time_now() + MAX_AUTH_CODE_TIME };
		spdlog::debug("time to live set to '{}'", std::to_string(time_to_live));

		std::unordered_map<std::string, std::string> query_params = util::extract_query_params(query_string);

		auto state_hash = query_params["state"];
		auto auth_code = query_params["code"];

		if (state_hash.empty() || auth_code.empty()) {  // Keys are not found in map
			spdlog::error("OauthManager: State or authorisation code could not be found in query string");
			throw std::runtime_error("OAuthManager: State or authorisation code could not be found in query string");
		} else {
			spdlog::info("OauthManager: state hash and authorisation code extracted");
		}

		auto item_map = auth_database_->get(session_id);
		std::string stored_hash = item_map["state_hash"].GetS();
		std::string code_verifier = item_map["code_verifier"].GetS();

		if (state_hash_manager_->check_state_hash(state_hash, stored_hash)) {
			set_state(State::AUTH_RECEIVED);
			spdlog::info("OauthManager: State hash is valid");
			spdlog::info("OauthManager: State set to AUTH_RECEIVED");
		}

		if (state_ != State::AUTH_RECEIVED) {
			spdlog::error("Stored state hash '{0}' does not match input state hash '{1}'", state_hash, state_hash);
			throw std::runtime_error("State hash does not match");
		} else {
			int64_t time_to_live{ get_time_now() + MAX_AUTH_CODE_TIME };
#if AXEL_TEST  // We can't actually exchange tokens with the real authorisation server during testing
			auto access_token = "dummy_access_token";

			spdlog::debug("OauthManager::receive_auth - Using dummy access token and time to live."
			              "time_to_live -  {}",
			              std::to_string(time_to_live));
#else
			auto access_token = token_request_manager_->send_token_request(auth_code,
			                                                               code_verifier,
			                                                               std::make_unique<util::HttpSender>());
#endif
			spdlog::info("OauthManager: Token request sent");

			set_state(State::TOKENS_RECEIVED);
			spdlog::info("OAuthManager: State set to TOKENS_RECEIVED");

			std::string session_token = session_manager_->get_session_token();
			spdlog::info("OAuthManager: Session ID for app obtained");

			// Store session details in oauth_app
			if (!app_database_->put({ { "session_id", session_token },
			                          { "access_token", access_token },
			                          { "time_to_live", std::to_string(time_to_live) } })) {
				throw std::runtime_error("Failed to put session_id, access_token into app database");
			} else {
				spdlog::info("OAuthManager: Stored (client session id, access token, time to live) in App table");
			};

			set_state(State::SESSION_ESTABLISHED);
			spdlog::info("OAuthManager: State set to SESSION_ESTABLISHED");

			return session_token;
		}
	}

	void auth::OauthManager::set_state(OauthManager::State state) {
		state_ = state;
	}
}  // namespace auth
