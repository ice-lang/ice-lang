#include "objects.hpp"

#include "../runtime/scope.hpp"

using namespace std;

namespace anole
{
Address EnumObject::load_member(const String &name)
{
    if (scope_->symbols().count(name))
    {
        return Allocator<Variable>::alloc(scope_->load_symbol(name)->sptr());
    }
    return Object::load_member(name);
}

void EnumObject::collect(function<void(Scope *)> func)
{
    func(scope_.get());
}
}
