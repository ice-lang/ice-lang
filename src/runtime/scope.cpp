#include "runtime.hpp"

#include "../objects/objects.hpp"

using namespace std;

namespace anole
{
Scope::Scope()
  : pre_scope_(nullptr)
{
    // ...
}

Scope::Scope(SPtr<Scope> pre_scope)
  : pre_scope_(std::move(pre_scope))
{
    // ...
}

SPtr<Scope> &Scope::pre()
{
    return pre_scope_;
}

Address Scope::create_symbol(const String &name)
{
    if (!symbols_.count(name))
    {
        symbols_[name] = Allocator<Variable>::alloc();
    }
    return symbols_[name];
}

void Scope::create_symbol(const String &name, Address value)
{
    symbols_[name] = value;
}

Address Scope::load_symbol(const String &name)
{
    auto ptr = find_symbol(name);
    auto res = ptr ? ptr : load_builtin(name);
    return res ? res : create_symbol(name);
}

Address Scope::load_builtin(const String &name)
{
    if (auto func = BuiltInFunctionObject::load_built_in_function(name))
    {
        return Allocator<Variable>::alloc(func);
    }
    return nullptr;
}

const std::map<String, Address> &Scope::symbols() const
{
    return symbols_;
}

Address Scope::find_symbol(const String &name)
{
    auto res = symbols_.find(name);
    if (res != symbols_.end())
    {
        return res->second;
    }
    else if (pre_scope_)
    {
        return pre_scope_->find_symbol(name);
    }
    else
    {
        return nullptr;
    }
}
}
