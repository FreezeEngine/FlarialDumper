#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <libhat/Process.hpp>
#include <libhat/Scanner.hpp>

int main() {
    std::cout << "Reading file..." << std::endl;
    // Open the file
    std::ifstream file(R"(L:\MC Bedrock Launcher\data\versions\cc2071ae-99fd-4112-b1e2-ff8ce34b9f6d\Minecraft.Windows.exe)", std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    // Determine the size of the file
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    if (fileSize < 0) {
        std::cerr << "Error determining file size!" << std::endl;
        return 1;
    }

    // Allocate memory
    std::vector<std::byte> data(fileSize);

    // Read the contents of the file into memory
    file.read(reinterpret_cast<char*>(data.data()), fileSize);

    if (!file) {
        std::cerr << "Error reading file!" << std::endl;
        return 1;
    }

    // Close the file
    file.close();

    std::byte* dataPtr = data.data();
    std::size_t dataSize = data.size();

    std::cout << "File read." << std::endl;
    std::cout << "Size: " << dataSize << std::endl;
    std::cout << "Searching..." << std::endl;

    auto module = hat::process::module_at(reinterpret_cast<uintptr_t>(dataPtr));
    auto signature = "C4 20 5F C3 CC CC 8B 81 ? ? ? ? C3"; // Actor::getCategories
    auto parsed_signature = hat::parse_signature(signature).value();
    const auto result =  hat::find_pattern(parsed_signature, ".text", module);

    if (result.has_result()) {
        auto pointer = reinterpret_cast<uintptr_t>(result.get()) + 8;
        auto offset = *reinterpret_cast<int*>(pointer);

        std::cout << "Name: Actor::getCategories" << std::endl;
        std::cout << "Address: 0x" << std::uppercase << std::hex << pointer << std::endl;
        std::cout << "Offset: 0x" << std::uppercase << std::hex << offset << std::endl;
        return 0;
    } else {
        std::cout << "Offset was not found!";
        return 0;
    }

    return 0;
}
