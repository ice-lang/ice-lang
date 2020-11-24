#ifndef __ANOLE_OBJECTS_CLASS_HPP__
#define __ANOLE_OBJECTS_CLASS_HPP__

#include "object.hpp"

namespace anole
{
class Scope;

class ClassObject : public Object
{
  public:
    ClassObject(String name,
        SPtr<Scope> pre_scope
    );

    Address load_member(const String &name) override;
    void call(Size num) override;

    void collect(std::function<void(Scope *)>) override;

    SPtr<Scope> &scope() { return scope_; }

  private:
    String name_;
    SPtr<Scope> scope_;
};
} // namespace anole

#endif
