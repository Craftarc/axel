#include "axel/ResourceManager.h"

#include <fstream>

#include <gmock/gmock.h>

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
}  // namespace axel