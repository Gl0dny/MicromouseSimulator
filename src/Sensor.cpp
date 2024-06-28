#include "Sensor.h"
#include <iostream>
#include "Maze.h"
#include "Logger.h"

Sensor::Sensor(Maze* maze, const std::string& name)
    : maze(maze), logger(std::make_unique<Logger>("logs/" + name + ".log")) {
    directionNames = {
        {{-1, 0}, "West"},
        {{1, 0}, "East"},
        {{0, -1}, "South"},
        {{0, 1}, "North"},
        {{-1, -1}, "South-West"},
        {{1, -1}, "South-East"},
        {{-1, 1}, "North-West"},
        {{1, 1}, "North-East"}
    };
    logger->enableFileOutput();
    logger->clearLogFile();
}

Sensor::~Sensor() {
    logger->disableFileOutput();
}

DistanceSensor::DistanceSensor(Maze* maze)
    : Sensor(maze, "distance_sensor") {}

void DistanceSensor::getSensorData(int x, int y, std::vector<std::vector<int>>& knownMaze, int step) const {
    knownMaze[x][y] = 0;
    for (const auto& [coordinates, direction] : directionNames) {
        if (direction == "West" || direction == "East" || direction == "South" || direction == "North") {
            int nx = x + coordinates.first;
            int ny = y + coordinates.second;

            if (nx >= 0 && ny >= 0 && nx < knownMaze.size() && ny < knownMaze[0].size()) {
                if (maze->isWall(nx, ny)) {
                    knownMaze[nx][ny] = 1;
                    logger->logMessage("Step " + std::to_string(step) + ": Wall detected to the " + direction + " at (" + std::to_string(nx) + ", " + std::to_string(ny) + ")");
                } else {
                    knownMaze[nx][ny] = 0;
                    logger->logMessage("Step " + std::to_string(step) + ": No wall detected to the " + direction + " at (" + std::to_string(nx) + ", " + std::to_string(ny) + ")");
                }
            } else {
                logger->logMessage("Step " + std::to_string(step) + ": Out of maze bounds at (" + std::to_string(nx) + ", " + std::to_string(ny) + ")");
            }
        }
    }
}

LaserSensor::LaserSensor(Maze* maze)
    : Sensor(maze, "laser_sensor") {}

void LaserSensor::getSensorData(int x, int y, std::vector<std::vector<int>>& knownMaze, int step) const {
    if (x < 0 || y < 0 || x >= maze->getWidth() || y >= maze->getHeight()) {
        logger->logMessage("Step " + std::to_string(step) + ": Starting position out of bounds at (" + std::to_string(x) + ", " + std::to_string(y) + ")");
        return;
    }

    knownMaze[x][y] = 0;

    for (const auto& [coordinates, direction] : directionNames) {
        if (direction == "West" || direction == "East" || direction == "South" || direction == "North") {
            int nx = x;
            int ny = y;

            while (true) {
                int nextX = nx + coordinates.first;
                int nextY = ny + coordinates.second;

                if (nextX < 0 || nextY < 0 || nextX >= maze->getWidth() || nextY >= maze->getHeight()) {
                    logger->logMessage("Step " + std::to_string(step) + ": Out of bounds to the " + direction + " at (" + std::to_string(nextX) + ", " + std::to_string(nextY) + ")");
                    break;
                }

                if (maze->isWall(nextX, nextY)) {
                    knownMaze[nextX][nextY] = 1;
                    logger->logMessage("Step " + std::to_string(step) + ": Laser detected wall to the " + direction + " at (" + std::to_string(nextX) + ", " + std::to_string(nextY) + ")");
                    break;
                }

                nx = nextX;
                ny = nextY;
                knownMaze[nx][ny] = 0;
                logger->logMessage("Step " + std::to_string(step) + ": Laser detected no wall to the " + direction + " at (" + std::to_string(nx) + ", " + std::to_string(ny) + ")");
            }
        }
    }
}

LidarSensor::LidarSensor(Maze* maze)
    : Sensor(maze, "lidar_sensor") {}

void LidarSensor::getSensorData(int x, int y, std::vector<std::vector<int>>& knownMaze, int step) const {
    knownMaze[x][y] = 0;
    for (int dx = -2; dx <= 2; ++dx) {
        for (int dy = -2; dy <= 2; ++dy) {
            if (dx == 0 && dy == 0) continue; // Skip the current position
            
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && ny >= 0 && nx < maze->getWidth() && ny < maze->getHeight()) {
                if (maze->isWall(nx, ny)) {
                    knownMaze[nx][ny] = 1;
                    logger->logMessage("Step " + std::to_string(step) + ": Lidar detected wall at (" + std::to_string(nx) + ", " + std::to_string(ny) + ")");
                } else {
                    knownMaze[nx][ny] = 0;
                    logger->logMessage("Step " + std::to_string(step) + ": Lidar detected no wall at (" + std::to_string(nx) + ", " + std::to_string(ny) + ")");
                }

                std::string direction;
                if (dx == 0 && dy == 0) {
                    direction = "Current position";
                } else if (dx == 0) {
                    direction = (dy > 0) ? "North" : "South";
                } else if (dy == 0) {
                    direction = (dx > 0) ? "East" : "West";
                } else if (dx > 0 && dy > 0) {
                    direction = "North-East";
                } else if (dx > 0 && dy < 0) {
                    direction = "South-East";
                } else if (dx < 0 && dy > 0) {
                    direction = "North-West";
                } else if (dx < 0 && dy < 0) {
                    direction = "South-West";
                }
                logger->logMessage("Step " + std::to_string(step) + ": to the " + direction);
            } else {
                std::string direction;
                if (dx == 0 && dy == 0) {
                    direction = "Current position";
                } else if (dx == 0) {
                    direction = (dy > 0) ? "North" : "South";
                } else if (dy == 0) {
                    direction = (dx > 0) ? "East" : "West";
                } else if (dx > 0 && dy > 0) {
                    direction = "North-East";
                } else if (dx > 0 && dy < 0) {
                    direction = "South-East";
                } else if (dx < 0 && dy > 0) {
                    direction = "North-West";
                } else if (dx < 0 && dy < 0) {
                    direction = "South-West";
                }
                logger->logMessage("Step " + std::to_string(step) + ": Out of bounds at (" + std::to_string(nx) + ", " + std::to_string(ny) + ") to the " + direction);
            }
        }
    }
}
