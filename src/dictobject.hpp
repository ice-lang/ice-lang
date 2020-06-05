#pragma once

#include <map>
#include "object.hpp"

namespace anole
{
class DictObject : public Object, public std::enable_shared_from_this<DictObject>
{
  public:
    struct ObjectCmp
    {
        bool operator()(const ObjectPtr &lhs, const ObjectPtr &rhs) const
        {
            return lhs->to_key() < rhs->to_key();
        }
    };

    using DataType = std::map<ObjectPtr, Address, ObjectCmp>;

    DictObject()
      : Object(ObjectType::Dict) {}

    bool to_bool() override;
    std::string to_str() override;
    std::string to_key() override;
    Address index(ObjectPtr) override;
    Address load_member(const std::string &name) override;

    DataType &data();
    void insert(ObjectPtr key, ObjectPtr value);

  private:
    DataType data_;
};
}
