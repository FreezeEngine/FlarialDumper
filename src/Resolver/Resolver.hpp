#pragma once
#include <string>
// Base class for resolving
class Resolver {
public:
    virtual int resolve(const std::basic_string<char> & signature, std::byte* dataPtr, int offset) = 0;
};
