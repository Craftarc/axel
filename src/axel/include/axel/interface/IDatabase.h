#ifndef AXEL_AXEL_IDATABASE_H
#define AXEL_AXEL_IDATABASE_H

namespace axel {
    class IDatabase {
    public:
        virtual ~IDatabase() = default;
        
        virtual bool put(const std::unordered_map<Aws::String, Aws::String>& items) = 0;
        
        [[nodiscard]] virtual Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>
        get(const Aws::String& key_value) const = 0;
        
        virtual bool del(const Aws::String& key_value) const = 0;
    };
} // axel

#endif //AXEL_AXEL_IDATABASE_H