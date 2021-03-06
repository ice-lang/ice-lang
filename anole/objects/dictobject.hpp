#ifndef __ANOLE_OBJECTS_DICT_HPP__
#define __ANOLE_OBJECTS_DICT_HPP__

#include "object.hpp"

#include <map>

namespace anole
{
class DictObject : public Object
{
  public:
    struct ObjectCmp
    {
        bool operator()(Object *lhs, Object *rhs) const;
    };

    using DataType = std::map<Object *, Address, ObjectCmp>;

  public:
    DictObject() noexcept;

    DataType &data();
    void insert(Object *key, Object *value);

  public:
    bool to_bool() override;
    String to_str() override;
    String to_key() override;

    Address index(Object *) override;
    Address load_member(const String &name) override;

    void collect(std::function<void(Object *)>) override;

  private:
    DataType data_;
};
}

#endif
