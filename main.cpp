#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <libhat/Process.hpp>
#include <nlohmann/json.hpp>
#include <sstream>
#include "src/Resolver/Resolvers/SignatureResolver/SignatureResolver.hpp"
#include "src/Resolver/Resolvers/VtableResolver/VtableResolver.hpp"
#include "src/Utils/Utils.hpp"

using json = nlohmann::json;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <path_to_executable> <path_to_config_file> <output_file_name>" << std::endl;
        return 1;
    }

    std::string executablePath = argv[1];
    std::string configFilePath = argv[2];
    std::string outputFileName = argv[3];

    // Read executable file
    std::ifstream file(executablePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<std::byte> data(fileSize);
    file.read(reinterpret_cast<char*>(data.data()), fileSize);
    file.close();
    std::byte* dataPtr = data.data();

    // Read configuration from JSON file
    json config = Utils::readConfigFile(configFilePath);

    // Create appropriate resolver and process each configuration object
    for (const auto& obj : config) {
        std::string name = obj["name"];
        std::string signature = obj["sig"];
        std::string outputFormat = obj.value("output", "%NAME%: %OFFSET%");
        int offset = obj.value("offset", 3); // Default offset is 3
        std::unique_ptr<Resolver> resolver;
        if (obj.value("type", "sig") == "vtable") {
            resolver = std::make_unique<VtableResolver>();
        } else {
            resolver = std::make_unique<SignatureResolver>();
        }
        int resolvedOffset = resolver->resolve(signature, dataPtr, offset);
        if (resolvedOffset != -1) {
            Utils::outputOffsetInfo(name, resolvedOffset, outputFormat, outputFileName);
        } else {
            std::ofstream outputFile("error_log.txt", std::ios_base::app);
            if (!outputFile.is_open()) {
                std::cerr << "Error opening output file!" << std::endl;
                return 1;
            }
            outputFile << "Offset for " << name << " was not found!" << std::endl;
            outputFile.close();
        }
    }

    return 0;
}
