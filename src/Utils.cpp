#include "Utils.h"
#include <random>
#include <fstream>
#include <iostream>
#include <ctime>
#include <sys/stat.h>

namespace Utils {

    int getRandomNumber(int min, int max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen);
    }

    void clearFile(const std::string &filePath) {
        std::ofstream ofs(filePath, std::ios::trunc);
        if (ofs) {
            std::cout << "File cleared: " << filePath << std::endl;
        } else {
            std::cerr << "Failed to clear file: " << filePath << std::endl;
        }
    }

    std::string getCurrentDateTime() {
        std::time_t now = std::time(nullptr);
        char buf[80];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        return buf;
    }

    void createDirectory(const std::string &dirPath) {
        struct stat info;

        if (stat(dirPath.c_str(), &info) != 0) {
            // Directory does not exist
            if (mkdir(dirPath.c_str(), 0777) == -1) {
                std::cerr << "Error creating directory: " << dirPath << std::endl;
            } else {
                std::cout << "Created directory: " << dirPath << std::endl;
            }
        } else if (!(info.st_mode & S_IFDIR)) {
            std::cerr << dirPath << " is not a directory" << std::endl;
        }
    }

    void fileExists(const std::string &filePath) {
        std::ifstream ifs(filePath);
        if (std::ifstream(filePath)) {
        std::cout << "File exists." << std::endl;
        } else {
            std::cout << "File does not exist." << std::endl;
        }
    }

} // namespace Utils
