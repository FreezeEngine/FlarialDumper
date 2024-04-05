#include <sstream>
#include "nlohmann/json.hpp"
#include "libhat/Scanner.hpp"
#include "libhat/Process.hpp"
#include <fstream>
#include <iostream>
#include "Utils.hpp"

using json = nlohmann::json;

// Helper function to read configuration from JSON file
json Utils::readConfigFile(const std::string& filePath) {
    std::ifstream configFile = openFile(filePath);
    json config;
    configFile >> config;
    return config;
}

std::ifstream Utils::openFile(const std::string &filePath) {
    std::ifstream configFile(filePath);
    if (!configFile.is_open()) {
        std::cerr << "Error opening config file!" << std::endl;
        exit(1);
    }
    return configFile;
}

// Helper function to output offset information to console and file
void Utils::outputOffsetInfo(const std::string& name, int offset, const std::string& outputFormat, const std::string& outputFileName) {
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