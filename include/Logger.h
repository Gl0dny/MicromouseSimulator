#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>

/**
 * @brief Logger class for logging messages to a file and/or console.
 */
class Logger {
public:
    /**
     * @brief Constructor for Logger.
     * @param filePath Path to the log file.
     */
    Logger(const std::string &filePath);

    /**
     * @brief Logs a message.
     * @param message The message to log.
     * @param includeTimestamp Whether to include a timestamp.
     * @return Reference to the Logger object.
     */
    Logger& logMessage(const std::string& message, bool includeTimestamp = true);

    /**
     * @brief Enables file output for logging.
     * @param toFileOnly Whether to log only to the file.
     * @return Reference to the Logger object.
     */
    Logger& enableFileOutput(bool toFileOnly = true);

    /**
     * @brief Disables file output for logging.
     * @return Reference to the Logger object.
     */
    Logger& disableFileOutput();

    /**
     * @brief Clears the log file.
     * @return Reference to the Logger object.
     */
    Logger& clearLogFile();

private:
    bool logToFile; ///< Indicates if logging to file is enabled.
    bool logToFileOnly; ///< Indicates if logging to file only is enabled.
    std::ofstream logFile; ///< Output file stream for logging.
    std::string logFilePath; ///< Path to the log file.

    /**
     * @brief Creates the log directory if it doesn't exist.
     * @param filePath Path to the log file.
     */
    void createLogDirectory(const std::string &filePath);
};

#endif // LOGGER_H
