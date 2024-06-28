#include "Simulator.h"
#include "Maze.h"
#include "Micromouse.h"
#include <iostream>
#include <memory>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>

/**
 * @brief CommandQueue class to handle command input in a thread-safe manner. 
 */
class CommandQueue {
public:
    /**
     * @brief Pushes a command onto the queue.
     * @param command The command to be pushed.
     */
    void push(const std::string& command) {
        std::lock_guard<std::mutex> lock(mtx);
        commands.push(command);
        cv.notify_one();
    }

    /**
     * @brief Pops a command from the queue.
     * @param command Reference to store the popped command.
     * @return True if a command was successfully popped, false otherwise.
     */
    bool pop(std::string& command) {
        std::unique_lock<std::mutex> lock(mtx);
        if (cv.wait_for(lock, std::chrono::milliseconds(100), [this] { return !commands.empty(); })) {
            command = commands.front();
            commands.pop();
            return true;
        }
        return false;
    }

private:
    std::queue<std::string> commands; ///< Queue to store commands.
    std::mutex mtx; ///< Mutex for thread safety.
    std::condition_variable cv; ///< Condition variable for synchronization.
};

/**
 * @brief Main function for the Micromouse simulation program. The main function orchestrates the Micromouse simulation by managing user input through CommandQueue, initializing and interacting with the Maze and Micromouse objects, and controlling simulation flow through threads (inputThread and simulationThread). It ensures thread-safe command handling and logging of simulation activities, providing a structured approach to simulate and control Micromouse behavior in a maze environment.
 * @return Exit status code.
 */
int main() {
    auto main_log_file = "./logs/main.log";
    auto logger = std::make_unique<Logger>(main_log_file);
    logger->enableFileOutput(false).clearLogFile().logMessage("Creating the Maze...");
    Maze* maze = Maze::getInstance();
    maze->displayMaze();
    maze->setLogger(main_log_file, false).displayMaze();

    logger->logMessage("Creating the Micromouse...");
    auto micromouse = chooseMicromouse(maze);
    if (!micromouse) {
        return 1;
    }

    logger->logMessage("Creating the Simulator...");
    auto simulator = std::make_unique<Simulator>(micromouse, maze);

    std::atomic<bool> exitFlag(false);
    CommandQueue commandQueue;

    logger->logMessage("Running the simulation. Started a thread to handle user input for start/pause/reset/exit");

    std::thread inputThread([&commandQueue, &exitFlag]() {
        std::string command;
        std::cout << "Enter 'start' to start the simulation, 'pause' to pause it, 'reset' to reset it, or 'exit' to exit: \n";
        while (!exitFlag) {
            std::getline(std::cin, command);
            if (!command.empty()) {
                commandQueue.push(command);
                if (command == "exit") {
                    exitFlag = true;
                }
            }
        }
    });

    std::thread simulationThread([&simulator, &commandQueue, &exitFlag, &logger]() {
        std::string command;
        while (!exitFlag) {
            if (commandQueue.pop(command)) {
                if (command == "start") {
                    simulator->start();
                    std::cout << "Simulation started. You can pause it by entering 'pause'.\n";
                } else if (command == "pause") {
                    simulator->pause();
                } else if (command == "reset") {
                    simulator->reset();
                } else if (command == "exit") {
                    simulator->pause();
                    std::cout << "Exiting...\n";
                } else {
                    simulator->pause();
                    std::cout << "Unknown command. Please enter 'start', 'pause', 'reset', or 'exit'.\n";
                }
                logger->logMessage("Command executed: " + command);
            }
        }
    });

    inputThread.join();
    simulationThread.join();

    logger->logMessage("Simulation finished.");
    logger->disableFileOutput();
    return 0;
}
