#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Micromouse.h"
#include "Maze.h"
#include "Logger.h"
#include <memory>
#include <chrono>
#include <atomic>

/**
 * @class The Simulator class orchestrates the simulation of a micromouse navigating through a maze. It interfaces with the Micromouse and Maze classes to control the micromouse's movement, manage simulation state, and handle logging of simulation events. This structured approach allows for clear separation of concerns and facilitates effective simulation management and monitoring.
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
     * @brief Destructor for Simulator.
     */
    ~Simulator();

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

    std::shared_ptr<Micromouse> micromouse; ///< Shared pointer to the Micromouse.
    Maze* maze; ///< Pointer to the Maze.
    int startX, startY; ///< Starting coordinates of the Micromouse.
    int steps; ///< Number of steps taken by the Micromouse.
    std::chrono::steady_clock::time_point startTime; ///< Start time of the simulation.
    std::chrono::duration<double> totalSeconds; ///< Total simulation time.
    std::atomic<bool> running; ///< Indicates whether the simulation is running.
    std::unique_ptr<Logger> logger; ///< Logger for the simulation.
};

#endif // SIMULATOR_H
