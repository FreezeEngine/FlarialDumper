using json = nlohmann::json;

class Utils {
public:
    static json readConfigFile(const std::string& filePath);
    static void outputOffsetInfo(const std::string& name, int offset, const std::string& outputFormat, const std::string& outputFileName);
    static std::ifstream openFile(const std::string &filePath);
};