#pragma once
#include "../../Resolver.hpp"

// Derived class for resolving vtable offsets
class VtableResolver : public Resolver {
public:
    int resolve(const std::basic_string<char> & signature, std::byte* dataPtr, int offset) override;
};
