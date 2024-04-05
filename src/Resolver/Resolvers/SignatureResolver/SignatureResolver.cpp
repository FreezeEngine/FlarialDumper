#include "SignatureResolver.hpp"

#include <nlohmann/json.hpp>
#include <libhat/Scanner.hpp>
#include <libhat/Process.hpp>

int SignatureResolver::resolve(const std::string& signature, std::byte *dataPtr, const int offset) {
    const auto module = hat::process::module_at(reinterpret_cast<unsigned long long int>(dataPtr));
    auto parsed_signature = hat::parse_signature(signature).value();
    const auto result = find_pattern(parsed_signature, ".text", module);

    if (result.has_result()) {
        const auto pointer = reinterpret_cast<unsigned long long int>(result.get()) + offset;
        const auto resolved_offset = *reinterpret_cast<int*>(pointer);
        return resolved_offset;
    }
    return -1; // Offset not found
}