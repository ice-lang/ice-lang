#include "path.hpp"
#include "../../../src/context.hpp"

using namespace std;
namespace fs = filesystem;
using namespace anole;

extern "C"
{
vector<String> _FUNCTIONS
{
    "__current_path"s
};

void __current_path(Size n)
{
    if (n != 0)
    {
        throw RuntimeError("function current_path need no arguments");
    }

    theCurrentContext
        ->push(
            make_shared<PathObject>(
                fs::current_path()
            )
        );
}
}

namespace
{

}

PathObject::PathObject(fs::path path)
  : path_(move(path)) {}

Address PathObject::load_member(const String &name)
{
    return nullptr;
}

String PathObject::to_str()
{
    return path_.string();
}

fs::path &PathObject::path()
{
    return path_;
}