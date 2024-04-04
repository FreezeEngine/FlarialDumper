#pragma once

#include "../../Resolver.hpp"

// Derived class for resolving vtable offsets
class VtableResolver : public Resolver {
public:
    int resolve(const std::string& signature, std::byte* dataPtr, int offset) override;
};