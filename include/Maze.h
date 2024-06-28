#ifndef MAZE_H
#define MAZE_H

#include "Logger.h"
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <utility>

/**
 * @brief Singleton class representing a Maze. The Maze class encapsulates maze generation and manipulation logic, providing methods to retrieve maze dimensions, display the maze, access maze grid data, and manage logging of maze operations. It employs the singleton pattern to ensure a single instance exists throughout the program's execution, enhancing consistency and control over maze generation and logging activities. This design facilitates organized development and debugging of maze-related applications.
 */
class Maze {
public:
    /**
     * @brief Destructor for Maze.
     */
    ~Maze();

    /**
     * @brief Returns the single instance of Maze.
     * @return Pointer to the Maze instance.
     */
    static Maze* getInstance();

    // Delete copy constructor and assignment operator to prevent duplicates
    Maze(Maze const &) = delete;
    void operator=(Maze const &) = delete;

    /**
     * @brief Gets the width of the maze.
     * @return Width of the maze.
     */
    int getWidth() const;

    /**
     * @brief Gets the height of the maze.
     * @return Height of the maze.
     */
    int getHeight() const;

    /**
     * @brief Displays the maze.
     */
    void displayMaze() const;

    /**
     * @brief Gets the maze grid.
     * @return Reference to the maze grid.
     */
    const std::vector<std::vector<int>>& getMazeGrid() const;

    /**
     * @brief Reads the exit coordinates of the maze.
     * @return Pair of exit coordinates (x, y).
     */
    std::pair<int, int> readExit() const;

    /**
     * @brief Checks if a cell is a wall.
     * @param x X-coordinate.
     * @param y Y-coordinate.
     * @return True if the cell is a wall, false otherwise.
     */
    bool isWall(int x, int y) const;

    /**
     * @brief Sets the logger for the maze.
     * @param logFile Path to the log file.
     * @param toFileOnly If true, log only to file.
     * @return Reference to the Maze instance.
     */
    Maze& setLogger(const std::string& logFile, bool toFileOnly = true);

private:
    /**
     * @brief Private constructor to prevent instantiation.
     */
    Maze();

    static Maze* instance; ///< Static member pointer to hold the instance.
    int width; ///< Width of the maze.
    int height; ///< Height of the maze.
    std::vector<std::vector<int>> mazeGrid; ///< Grid representing the maze.
    std::pair<int, int> exit; ///< Coordinates of the exit.
    std::unique_ptr<Logger> logger; ///< Logger object.
    std::map<std::pair<int, int>, std::string> directionNames; ///< Map of direction names.

    /**
     * @brief Generates the maze.
     */
    void generateMaze();

    /**
     * @brief Carves passage at given coordinates.
     * @param x X-coordinate.
     * @param y Y-coordinate.
     */
    void carvePassage(int x, int y);

    /**
     * @brief Creates a random exit on the maze border.
     */
    void createRandomExit();

    /**
     * @brief Checks if the exit is valid.
     * @param x X-coordinate.
     * @param y Y-coordinate.
     * @return True if the exit is valid, false otherwise.
     */
    bool isValidExit(int x, int y);

    /**
     * @brief Prints the maze with the current position of the carving algorithm.
     * @param cx Current x-coordinate.
     * @param cy Current y-coordinate.
     */
    void printMazeWithCurrentCarve(int cx, int cy) const;
};

#endif // MAZE_H
