#ifndef SENSOR_H
#define SENSOR_H

#include <vector>
#include <memory>
#include <map>
#include <string>
#include <utility>
#include "Maze.h"
#include "Logger.h"

/**
 * @brief The Logger class encapsulates functionality for logging messages with optional timestamps to a specified file path. It supports enabling/disabling file output, clearing the log file, and managing logging behavior between file and console outputs. This setup facilitates organized logging in applications, aiding in debugging, monitoring, and analysis tasks. The class ensures efficient file handling and robust logging capabilities within a program.
 */
class Sensor {
public:
    /**
     * @brief Constructor for Sensor.
     * @param maze Pointer to the Maze object.
     * @param name Name of the sensor.
     */
    Sensor(Maze* maze, const std::string& name);

    /**
     * @brief Virtual destructor for Sensor.
     */
    virtual ~Sensor();

    /**
     * @brief Pure virtual function to get sensor data.
     * @param x X-coordinate.
     * @param y Y-coordinate.
     * @param knownMaze 2D vector representing the known maze.
     * @param step Current step number.
     */
    virtual void getSensorData(int x, int y, std::vector<std::vector<int>>& knownMaze, int step) const = 0;

protected:
    Maze* maze; ///< Pointer to the maze object.
    std::map<std::pair<int, int>, std::string> directionNames; ///< Map of direction names.
    std::unique_ptr<Logger> logger; ///< Logger object.
    int steps; ///< Step counter.
};

/**
 * @brief The DistanceSensor class provides functionality to scan orthogonal directions from a starting position (x, y) within a maze. It uses a Maze object to query the environment and updates a 2D vector (knownMaze) to maintain a representation of the maze's layout based on sensor readings. 
 */
class DistanceSensor : public Sensor {
public:
    /**
     * @brief Constructor for DistanceSensor.
     * @param maze Pointer to the Maze object.
     */
    DistanceSensor(Maze* maze);

    /**
     * @brief The getSensorData function handles scanning for walls, logs detection results, and manages boundary conditions to ensure accurate mapping of the maze environment.
     * @param x X-coordinate.
     * @param y Y-coordinate.
     * @param knownMaze 2D vector representing the known maze.
     * @param step Current step number.
     */
    void getSensorData(int x, int y, std::vector<std::vector<int>>& knownMaze, int step) const override;
};

/**
 * @brief The LaserSensor class provides functionality to scan multiple directions from a position (x, y) within a maze until the reading meets a wall. It uses a Maze object to query the environment and updates a 2D vector (knownMaze) to maintain a representation of the maze's layout based on sensor readings. 
 */ 
class LaserSensor : public Sensor {
public:
    /**
     * @brief Constructor for LaserSensor.
     * @param maze Pointer to the Maze object.
     */
    LaserSensor(Maze* maze);

    /**
     * @brief The getSensorData function handles scanning for walls in orthogonal directions, logs detection results, and manages boundary conditions, ensuring accurate mapping of the maze environment.
     * @param x X-coordinate.
     * @param y Y-coordinate.
     * @param knownMaze 2D vector representing the known maze.
     * @param step Current step number.
     */
    void getSensorData(int x, int y, std::vector<std::vector<int>>& knownMaze, int step) const override;
};

/**
 * @brief The LidarSensor class provides functionality to detect walls and open spaces around a specific coordinate in a maze. It uses a Maze object to query the environment and updates a 2D vector (knownMaze) to maintain a representation of the maze's layout based on sensor readings.
 */
class LidarSensor : public Sensor {
public:
    /**
     * @brief Constructor for LidarSensor.
     * @param maze Pointer to the Maze object.
     */
    LidarSensor(Maze* maze);

    /**
     * @brief The getSensorData function handles adjacent positions, logs detection results, and manages boundary conditions, ensuring accurate mapping of the maze environment.
     * @param x X-coordinate.
     * @param y Y-coordinate.
     * @param knownMaze 2D vector representing the known maze.
     * @param step Current step number.
     */
    void getSensorData(int x, int y, std::vector<std::vector<int>>& knownMaze, int step) const override;
};

#endif // SENSOR_H
