#ifndef UTILS_H
#define UTILS_H

#include <string>

/**
 * @namespace Utils
 * @brief Utility functions for various tasks.
 */
namespace Utils {

    /**
     * @brief Generates a random number between min and max.
     * @param min Minimum value of the random number.
     * @param max Maximum value of the random number.
     * @return A random integer between min and max.
     */
    int getRandomNumber(int min, int max);

    /**
     * @brief Gets the current date and time as a string.
     * @return A string representing the current date and time.
     */
    std::string getCurrentDateTime();

    /**
     * @brief Creates a directory if it does not already exist.
     * @param dirPath Path to the directory.
     */
    void createDirectory(const std::string &dirPath);

    /**
     * @brief Clears the contents of a file.
     * @param filePath Path to the file to be cleared.
     */
    void clearFile(const std::string &filePath);

    /**
     * @brief Checks if a file exists.
     * @param filePath Path to the file.
     */
    void fileExists(const std::string &filePath);
}

#endif // UTILS_H
