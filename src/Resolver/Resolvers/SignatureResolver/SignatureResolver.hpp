#pragma once

#include "../../Resolver.hpp"

// Derived class for resolving signatures
class SignatureResolver : public Resolver {
public:
    int resolve(const std::string& signature, std::byte* dataPtr, int offset) override;
};