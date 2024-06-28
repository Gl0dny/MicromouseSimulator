#ifndef MICROMOUSE_H
#define MICROMOUSE_H

#include "Maze.h"
#include "Sensor.h"
#include "Logger.h"
#include <memory>
#include <vector>
#include <string>
#include <set>
#include <stack>
#include <utility>

/**
 * @brief The Micromouse class encapsulates the core functionality and state management for a micromouse navigating and mapping a maze. It provides methods for setting up sensors, making decisions based on sensor data, moving within the maze, and maintaining an updated map (knownMaze). The class also handles initialization, resetting to initial state, and logging of micromouse activities, facilitating systematic maze exploration and navigation. This modular design supports the implementation of various micromouse algorithms by deriving classes that implement specific decision-making strategies (makeDecision()).
 */
class Micromouse {
public:
    /**
     * @brief Constructor for Micromouse.
     * @param logFileName Name of the log file.
     */
    Micromouse(const std::string& logFileName);

    /**
     * @brief Virtual destructor for Micromouse.
     */
    virtual ~Micromouse();

    /**
     * @brief Sets the sensor for the Micromouse.
     * @param sensor Shared pointer to a Sensor object.
     */
    void setSensor(std::shared_ptr<Sensor> sensor);

    /**
     * @brief Makes a decision on the next move.
     */
    virtual void makeDecision() = 0;

    /**
     * @brief Gets the current X position of the Micromouse.
     * @return X position.
     */
    int getPosX() const;

    /**
     * @brief Gets the current Y position of the Micromouse.
     * @return Y position.
     */
    int getPosY() const;

    /**
     * @brief Sets the position of the Micromouse.
     * @param x X position.
     * @param y Y position.
     */
    void setPosition(int x, int y);

    /**
     * @brief Moves the Micromouse based on its current decision.
     */
    void move();

    /**
     * @brief Reads the sensor data and updates the known maze.
     */
    void readSensors();

    /**
     * @brief Gets the current step count.
     * @return Step count.
     */
    int getStep() const;

    /**
     * @brief Gets the known maze.
     * @return Known maze grid.
     */
    std::vector<std::vector<int>> getKnownMaze() const;

    /**
     * @brief Initializes the known maze with given dimensions.
     * @param width Width of the maze.
     * @param height Height of the maze.
     */
    void initializeKnownMaze(int width, int height);
    
    /**
     * @brief Resets the Micromouse to its initial state.
     */
    void reset();

protected:
    int posX, posY; ///< Current X and Y positions of the Micromouse.
    std::string direction; ///< Current direction of the Micromouse.
    std::shared_ptr<Sensor> sensor; ///< Sensor for the Micromouse.
    std::vector<std::vector<int>> knownMaze; ///< Known maze grid.
    std::unique_ptr<Logger> logger; ///< Logger object.
    const std::map<std::string, std::pair<int, int>> directions = {
        {"North", {0, 1}}, {"East", {1, 0}}, {"South", {0, -1}}, {"West", {-1, 0}}
    }; ///< Direction map.
    const std::map<std::string, std::string> rightTurns = {
        {"North", "East"}, {"East", "South"}, {"South", "West"}, {"West", "North"}
    }; ///< Right turns map.
    const std::map<std::string, std::string> leftTurns = {
        {"North", "West"}, {"West", "South"}, {"South", "East"}, {"East", "North"}
    }; ///< Left turns map.
    int step; ///< Current step count.
};

/**
 * @brief The RightHandRuleBacktrackingMazeSolver class extends Micromouse and implements a maze-solving strategy based on the right-hand rule with backtracking. It leverages inherited methods and data members to manage micromouse state, decision-making based on sensor data (knownMaze), and logging of micromouse activities. This algorithmic approach ensures systematic maze exploration and backtracking when encountering dead-ends or blocked paths, aiming to find an optimal route through the maze environment.
 */
class RightHandRuleBacktrackingMazeSolver : public Micromouse {
public:
    RightHandRuleBacktrackingMazeSolver();
    void makeDecision() override;

private:
    void followRightHandRule();
};

/**
 * @brief The LeftHandRuleBacktrackingMazeSolver class extends Micromouse and implements a maze-solving strategy based on the left-hand rule with backtracking. It utilizes inherited methods and data members to manage micromouse state, decision-making based on sensor data (knownMaze), and logging of micromouse activities. This algorithmic approach ensures systematic maze exploration and backtracking when encountering dead-ends or blocked paths, aiming to find an optimal route through the maze environment.
 */
class LeftHandRuleBacktrackingMazeSolver : public Micromouse {
public:
    LeftHandRuleBacktrackingMazeSolver();
    void makeDecision() override;

private:
    void followLeftHandRule();
};

/**
 * @brief The TeleportingUndecidedMazeSolver class extends Micromouse and implements a maze-solving strategy that combines teleporting with an undecided path-solving approach. It utilizes backtracking (backtrackStack) to revisit previous positions and attempt alternative paths when encountering dead-ends or fully explored areas. This strategy aims to efficiently explore and navigate through the maze while adapting to unknown configurations and obstacles encountered.
 */
class TeleportingUndecidedMazeSolver : public Micromouse {
public:
    TeleportingUndecidedMazeSolver();
    void makeDecision() override;

private:
    std::map<std::pair<int, int>, int> visited; ///< Tracks visited cells.
    std::map<std::pair<int, int>, std::set<std::string>> triedDirections; ///< Tracks tried directions from each cell.
    std::stack<std::pair<int, int>> backtrackStack; ///< Stack for backtracking.

    bool hasUntriedDirection(int x, int y);
    std::string getNextDirection(int x, int y);
};

/**
 * @brief Factory function to create a Micromouse with given solver and sensor types. The createMicromouse function provides a flexible mechanism to instantiate micromouse objects with different solver and sensor types, based on the template parameters SolverType and SensorType. This approach supports modular design and facilitates the creation of micromouse instances tailored to specific maze-solving algorithms and sensor capabilities, enhancing reusability and flexibility in micromouse simulation and development environments.
 * @tparam SolverType Type of the solver.
 * @tparam SensorType Type of the sensor.
 * @param maze Pointer to the maze object.
 * @return Shared pointer to the created Micromouse.
 */
template <typename SolverType, typename SensorType>
std::shared_ptr<Micromouse> createMicromouse(Maze* maze) {
    auto micromouse = std::make_shared<SolverType>();
    auto sensor = std::make_shared<SensorType>(maze);
    micromouse->initializeKnownMaze(maze->getWidth(), maze->getHeight());
    micromouse->setSensor(sensor);
    return micromouse;
}

/**
 * @brief Function to choose and create a Micromouse based on user input. The chooseMicromouse function provides a user-friendly interface for selecting and creating micromouse instances with different solver and sensor types dynamically. By leveraging templated createMicromouse function calls based on user input, it allows for flexible configuration of micromouse behavior to suit d ifferent maze-solving strategies and sensor capabilities. This approach supports modularity and extensibility in micromouse simulation and development, enabling rapid testing and iteration of different algorithms and sensor combinations within a maze environment.
 * @param maze Pointer to the maze object.
 * @return Shared pointer to the created Micromouse.
 */
std::shared_ptr<Micromouse> chooseMicromouse(Maze* maze);

#endif // MICROMOUSE_H
