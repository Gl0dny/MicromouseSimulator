#include "Micromouse.h"
#include <iostream>
#include <limits>

Micromouse::Micromouse(const std::string& logFileName)
    : posX(1), posY(1), direction("North"), step(0), logger(std::make_unique<Logger>("logs/" + logFileName + ".log")) {
    logger->enableFileOutput();
    logger->clearLogFile();
}

Micromouse::~Micromouse() {
    logger->disableFileOutput();
}

void Micromouse::setSensor(std::shared_ptr<Sensor> sensor) {
    this->sensor = sensor;
    logger->logMessage("Sensor set for Micromouse");
}

int Micromouse::getPosX() const {
    return posX;
}

int Micromouse::getPosY() const {
    return posY;
}

void Micromouse::setPosition(int x, int y) {
    posX = x;
    posY = y;
    logger->logMessage("Step " + std::to_string(step) + ": Micromouse position set to (" + std::to_string(x) + "," + std::to_string(y) + ")");
}

void Micromouse::move() {
    step++;
    readSensors();
    makeDecision();
    if (direction == "North") {
        posY++;
    } else if (direction == "East") {
        posX++;
    } else if (direction == "South") {
        posY--;
    } else if (direction == "West") {
        posX--;
    }
    logger->logMessage("Step " + std::to_string(step) + ": Micromouse moved to (" + std::to_string(posX) + "," + std::to_string(posY) + ") facing " + direction);
}

void Micromouse::readSensors() {
    sensor->getSensorData(posX, posY, knownMaze, step);
    logger->logMessage("Step " + std::to_string(step) + ": Sensors read at position (" + std::to_string(posX) + "," + std::to_string(posY) + ")");
}

int Micromouse::getStep() const {
    return step;
}

void Micromouse::initializeKnownMaze(int width, int height) {
    knownMaze = std::vector<std::vector<int>>(width, std::vector<int>(height, -1));
    logger->logMessage("Known maze initialized with dimensions (" + std::to_string(width) + "," + std::to_string(height) + ")");
}

std::vector<std::vector<int>> Micromouse::getKnownMaze() const {
    return knownMaze;
}

void Micromouse::reset() {
    posX = 1;
    posY = 1;
    direction = "North";
    for (auto& row : knownMaze) {
        std::fill(row.begin(), row.end(), -1);
    }
    logger->logMessage("Micromouse reset to initial state");
}

RightHandRuleBacktrackingMazeSolver::RightHandRuleBacktrackingMazeSolver()
    : Micromouse("rhrb_maze_solver") {}

void RightHandRuleBacktrackingMazeSolver::makeDecision() {
    followRightHandRule();
}

void RightHandRuleBacktrackingMazeSolver::followRightHandRule() {
    std::string newDirection = rightTurns.at(direction);
    auto [dx, dy] = directions.at(newDirection);

    if (knownMaze[posX + dx][posY + dy] != 1) {
        direction = newDirection;
    } else {
        auto [dx, dy] = directions.at(direction);
        if (knownMaze[posX + dx][posY + dy] == 1) {
            newDirection = leftTurns.at(direction);
            auto [dx, dy] = directions.at(newDirection);
            if (knownMaze[posX + dx][posY + dy] != 1) {
                direction = newDirection;
            } else {
                newDirection = rightTurns.at(rightTurns.at(direction));
                direction = newDirection;
            }
        }
    }
    logger->logMessage("Step " + std::to_string(step) + ": Following right-hand rule. Micromouse decided to turn " + direction);
}

LeftHandRuleBacktrackingMazeSolver::LeftHandRuleBacktrackingMazeSolver()
    : Micromouse("lhrb_maze_solver") {}

void LeftHandRuleBacktrackingMazeSolver::makeDecision() {
    followLeftHandRule();
}

void LeftHandRuleBacktrackingMazeSolver::followLeftHandRule() {
    std::string newDirection = leftTurns.at(direction);
    auto [dx, dy] = directions.at(newDirection);

    if (knownMaze[posX + dx][posY + dy] != 1) {
        direction = newDirection;
    } else {
        auto [dx, dy] = directions.at(direction);
        if (knownMaze[posX + dx][posY + dy] == 1) {
            newDirection = rightTurns.at(direction);
            auto [dx, dy] = directions.at(newDirection);
            if (knownMaze[posX + dx][posY + dy] != 1) {
                direction = newDirection;
            } else {
                newDirection = leftTurns.at(leftTurns.at(direction));
                direction = newDirection;
            }
        }
    }
    logger->logMessage("Step " + std::to_string(step) + ": Following left-hand rule. Micromouse decided to turn " + direction);
}

TeleportingUndecidedMazeSolver::TeleportingUndecidedMazeSolver()
    : Micromouse("teleporting_undecided_maze_solver") {}

bool TeleportingUndecidedMazeSolver::hasUntriedDirection(int x, int y) {
    for (const auto& dir : directions) {
        int newX = x + dir.second.first;
        int newY = y + dir.second.second;
        if (knownMaze[newX][newY] != 1 && triedDirections[{x, y}].find(dir.first) == triedDirections[{x, y}].end()) {
            return true;
        }
    }
    return false;
}

std::string TeleportingUndecidedMazeSolver::getNextDirection(int x, int y) {
    for (const auto& dir : directions) {
        int newX = x + dir.second.first;
        int newY = y + dir.second.second;
        if (knownMaze[newX][newY] != 1 && triedDirections[{x, y}].find(dir.first) == triedDirections[{x, y}].end()) {
            triedDirections[{x, y}].insert(dir.first);
            return dir.first;
        }
    }
    return "";
}

void TeleportingUndecidedMazeSolver::makeDecision() {
    int x = getPosX();
    int y = getPosY();
    visited[{x, y}] = 1;

    if (backtrackStack.empty() || backtrackStack.top() != std::make_pair(x, y)) {
        backtrackStack.push({x, y});
    }

    if (hasUntriedDirection(x, y)) {
        direction = getNextDirection(x, y);
        return;
    }

    while (!backtrackStack.empty()) {
        auto [backX, backY] = backtrackStack.top();
        backtrackStack.pop();

        if (hasUntriedDirection(backX, backY)) {
            direction = getNextDirection(backX, backY);
            setPosition(backX, backY);
            return;
        }
    }

    logger->logMessage("Step " + std::to_string(step) + ": Micromouse is stuck with no untried directions available.");
}

std::shared_ptr<Micromouse> chooseMicromouse(Maze* maze) {
    int solverChoice, sensorChoice;
    std::cout << "Choose Micromouse type:\n1. Right Hand Rule\n2. Left Hand Rule\n3. Teleporting Undecided Solver\n";
    std::cin >> solverChoice;
    std::cout << "Choose Sensor type:\n1. Distance Sensor\n2. Laser Sensor\n3. Lidar Sensor\n";
    std::cin >> sensorChoice;

    switch (solverChoice) {
        case 1:
            switch (sensorChoice) {
                case 1:
                    return createMicromouse<RightHandRuleBacktrackingMazeSolver, DistanceSensor>(maze);
                case 2:
                    return createMicromouse<RightHandRuleBacktrackingMazeSolver, LaserSensor>(maze);
                case 3:
                    return createMicromouse<RightHandRuleBacktrackingMazeSolver, LidarSensor>(maze);
                default:
                    std::cerr << "Invalid sensor choice" << std::endl;
                    return nullptr;
            }
        case 2:
            switch (sensorChoice) {
                case 1:
                    return createMicromouse<LeftHandRuleBacktrackingMazeSolver, DistanceSensor>(maze);
                case 2:
                    return createMicromouse<LeftHandRuleBacktrackingMazeSolver, LaserSensor>(maze);
                case 3:
                    return createMicromouse<LeftHandRuleBacktrackingMazeSolver, LidarSensor>(maze);
                default:
                    std::cerr << "Invalid sensor choice" << std::endl;
                    return nullptr;
            }
        case 3:
            switch (sensorChoice) {
                case 1:
                    return createMicromouse<TeleportingUndecidedMazeSolver, DistanceSensor>(maze);
                case 2:
                    return createMicromouse<TeleportingUndecidedMazeSolver, LaserSensor>(maze);
                case 3:
                    return createMicromouse<TeleportingUndecidedMazeSolver, LidarSensor>(maze);
                default:
                    std::cerr << "Invalid sensor choice" << std::endl;
                    return nullptr;
            }
        default:
            std::cerr << "Invalid solver choice" << std::endl;
            return nullptr;
    }
}
