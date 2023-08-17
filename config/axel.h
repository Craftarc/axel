#ifndef AXEL_CONFIG_AXEL_H
#define AXEL_CONFIG_AXEL_H

#include <aws/dynamodb/DynamoDBServiceClientModel.h>

namespace config {
    /// @brief Contains constants related to Axel's functionality
    namespace axel {
        const std::string client_id = "axel";
        const std::string grant_type = "authorization_code";
        namespace database {
            const std::string auth = "axel-oauth";
            const std::string auth_test = "axel-oauth-test";
            const std::unordered_map<Aws::String, Aws::DynamoDB::Model::ValueType> auth_attributes = {{"session_id",
                                                                                                       Aws::DynamoDB::Model::ValueType::STRING},
                                                                                                      {"state_hash",
                                                                                                       Aws::DynamoDB::Model::ValueType::STRING},
                                                                                                      {"code_verifier",
                                                                                                       Aws::DynamoDB::Model::ValueType::STRING},
                                                                                                      {"time_to_live",
                                                                                                       Aws::DynamoDB::Model::ValueType::NUMBER}};
            const std::string app = "axel-app";
            const std::string app_test = "axel-app-test";
            const std::unordered_map<Aws::String, Aws::DynamoDB::Model::ValueType> app_attributes = {{"session_id",
                                                                                                      Aws::DynamoDB::Model::ValueType::STRING},
                                                                                                     {"access_token",
                                                                                                      Aws::DynamoDB::Model::ValueType::STRING},
                                                                                                     {"time_to_live",
                                                                                                      Aws::DynamoDB::Model::ValueType::NUMBER}};
        }
    }
}

#endif // AXEL_CONFIG_AXEL_H