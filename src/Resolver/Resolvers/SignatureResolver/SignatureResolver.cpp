#include <sstream>
#include "nlohmann/json.hpp"
#include "libhat/Scanner.hpp"
#include "libhat/Process.hpp"
#include <iterator>
#include <iostream>
#include "SignatureResolver.hpp"

int SignatureResolver::resolve(const std::basic_string<char> &signature, std::byte *dataPtr, int offset) {
    auto module = hat::process::module_at(reinterpret_cast<unsigned long long int>(dataPtr));
    auto parsed_signature = hat::parse_signature(signature).value();
    const auto result =  hat::find_pattern(parsed_signature, ".text", module);

    if (result.has_result()) {
        auto pointer = reinterpret_cast<unsigned long long int>(result.get()) + offset;
        auto resolved_offset = *reinterpret_cast<int*>(pointer);
        return resolved_offset;
    } else {
        return -1; // Offset not found
    }
}
