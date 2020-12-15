#ifndef __ANOLE_ERROR_HPP__
#define __ANOLE_ERROR_HPP__

#include "base.hpp"

#include <exception>

namespace anole
{
namespace info
{
inline String strong(const String &str)
{
    return "\033[1m" + str + "\033[0m";
}

inline String warning(const String &str)
{
    return "\033[31m" + str + "\033[0m";
}
}

class CompileError : public std::exception
{
  public:
    CompileError(String err);
    CompileError &operator=(const CompileError &other) noexcept;

    const char *what() const noexcept override;

  private:
    String err_;
};

class RuntimeError : public std::exception
{
  public:
    RuntimeError(const String &err);
    RuntimeError &operator=(const RuntimeError &other) noexcept;

    const char *what() const noexcept override;

  private:
    String err_;
};
}

#endif