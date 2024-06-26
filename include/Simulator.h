#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Micromouse.h"
#include "Maze.h"
#include "Logger.h"
#include <memory>
#include <chrono>
#include <atomic>

/**
 * @class Simulator
 * @brief Controls the simulation of the Micromouse in the maze.
 */
class Simulator {
public:
    /**
     * @brief Constructs a new Simulator object.
     * @param micromouse Shared pointer to a Micromouse object.
     * @param maze Pointer to a Maze object.
     */
    Simulator(std::shared_ptr<Micromouse> micromouse, Maze* maze);

    /**
     * @brief Runs the simulation.
     */
    void run();

    /**
     * @brief Starts/unpauses the simulation.
     */
    void start();

    /**
     * @brief Pauses the simulation.
     */
    void pause();

    /**
     * @brief Resets the simulation.
     */
    void reset();

private:
    /**
     * @brief Sets a random start position for the Micromouse.
     */
    void setRandomStartPosition();

    /**
     * @brief Displays the maze with the mouse's current position.
     */
    void displayMazeWithMouse() const;

    /**
     * @brief Checks if the Micromouse has reached the goal.
     * @return true if the Micromouse has reached the goal, false otherwise.
     */
    bool hasReachedGoal() const;

    /**
     * @brief Checks and handles wall collisions.
     */
    void checkAndHandleWallCollision();

    std::shared_ptr<Micromouse> micromouse;
    Maze* maze;
    int startX, startY;
    int steps;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::duration<double> totalSeconds;
    std::atomic<bool> running;
    std::unique_ptr<Logger> logger;
};

#endif // SIMULATOR_H
