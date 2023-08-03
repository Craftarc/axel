#ifndef AXEL_AXEL_MOCKDATABASE_H
#define AXEL_AXEL_MOCKDATABASE_H

#include <unordered_map>

#include <gmock/gmock.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/core/utils/memory/stl/AWSMap.h>
#include <aws/dynamodb/model/AttributeValue.h>

#include "axel/interface/IDatabase.h"

namespace axel {
    class MockDatabase : public IDatabase {
    public:
        MOCK_METHOD(bool, put, ((const std::unordered_map<Aws::String, Aws::String>& items)), (override));
        
        MOCK_METHOD((Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>),
                    get,
                    (const Aws::String& key_value),
                    (const, override));
        
        MOCK_METHOD(bool, del, (const Aws::String& key_value), (const, override));
    };
}

#endif // AXEL_AXEL_MOCKDATABASE_H
