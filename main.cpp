#include "src/Resolver/Resolvers/VtableResolver/VtableResolver.hpp"
#include "src/Resolver/Resolvers/SignatureResolver/SignatureResolver.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <nlohmann/json.hpp>
#include <libhat/Scanner.hpp>

using json = nlohmann::json;

// Helper function to read configuration from JSON file
json readConfigFile(const std::string& filePath) {
    std::ifstream configFile(filePath);
    if (!configFile.is_open()) {
        std::cerr << "Error opening config file!" << std::endl;
        exit(1);
    }
    json config;
    configFile >> config;
    return config;
}

// Helper function to output offset information to console and file
void outputOffsetInfo(const std::string& name, int offset, const std::string& outputFormat, const std::string& outputFileName) {
    std::ofstream outputFile(outputFileName, std::ios_base::app);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file!" << std::endl;
        return;
    }
    std::ofstream outputLogFile(outputFileName+".log", std::ios_base::app);
    if (!outputLogFile.is_open()) {
        std::cerr << "Error opening output log file!" << std::endl;
        return;
    }

    std::string formattedOutput = outputFormat;
    size_t pos = formattedOutput.find("%NAME%");
    if (pos != std::string::npos)
        formattedOutput.replace(pos, 6, name);
    pos = formattedOutput.find("%OFFSET%");
    if (pos != std::string::npos) {
        std::stringstream stream;
        stream << "0x" << std::hex << offset;
        formattedOutput.replace(pos, 8, stream.str());
    }
    std::cout << name << ": " << "0x" << std::hex << offset << std::endl;
    outputLogFile << name << ": " << "0x" << std::hex << offset << std::endl;
    outputFile << formattedOutput << std::endl;

    outputFile.close();
}

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
    json config = readConfigFile(configFilePath);

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
            outputOffsetInfo(name, resolvedOffset, outputFormat, outputFileName);
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
