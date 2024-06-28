#include "Maze.h"
#include "Utils.h"
#include "Logger.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <map>
#include <vector>
#include <memory>

Maze* Maze::instance = nullptr;

Maze::Maze()
    : width(21), height(21), mazeGrid(width, std::vector<int>(height, 1)), logger(std::make_unique<Logger>("./logs/maze.log")) {
    directionNames = {
        {{-1, 0}, "West"},
        {{1, 0}, "East"},
        {{0, -1}, "South"},
        {{0, 1}, "North"}
    };
    logger->enableFileOutput();
    logger->clearLogFile();
    logger->logMessage("Maze initialized with all walls.");
    generateMaze();
}

Maze::~Maze() {
    logger->disableFileOutput();
    delete instance;
}

Maze* Maze::getInstance() {
    if (!instance) {
        instance = new Maze();
    }
    return instance;
}

int Maze::getWidth() const {
    return width;
}

int Maze::getHeight() const {
    return height;
}

void Maze::displayMaze() const {
    logger->logMessage("Displaying maze:");
    for (int y = height - 1; y >= 0; --y) {
        std::string rowString;
        for (int x = 0; x < width; ++x) {
            rowString += (mazeGrid[x][y] ? '#' : ' ');
            rowString += ' ';
        }
        logger->logMessage(rowString, false);
    }
}

const std::vector<std::vector<int>>& Maze::getMazeGrid() const {
    return mazeGrid;
}

std::pair<int, int> Maze::readExit() const {
    return exit;
}

bool Maze::isWall(int x, int y) const {
    return mazeGrid[x][y] == 1;
}

Maze& Maze::setLogger(const std::string& logFile, bool toFileOnly /* = true */) {
    logger = std::make_unique<Logger>(logFile);
    logger->enableFileOutput(toFileOnly);
    return *this;
}

void Maze::generateMaze() {
    logger->logMessage("Starting maze generation.");
    carvePassage(1, 1);
    createRandomExit();
    logger->logMessage("Maze generation completed.");
}

void Maze::createRandomExit() {
    std::vector<std::pair<int, int>> borderCells;
    for (int i = 1; i < width - 1; ++i) {
        borderCells.push_back({i, 0});
        borderCells.push_back({i, height - 1});
    }
    for (int i = 1; i < height - 1; ++i) {
        borderCells.push_back({0, i});
        borderCells.push_back({width - 1, i});
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<> dis(0, borderCells.size() - 1);

    bool exitCreated = false;
    while (!exitCreated && !borderCells.empty()) {
        int idx = dis(g);
        auto [exitX, exitY] = borderCells[idx];
        borderCells.erase(borderCells.begin() + idx);

        if (isValidExit(exitX, exitY)) {
            mazeGrid[exitX][exitY] = 0;
            logger->logMessage("Created exit at (" + std::to_string(exitX) + ", " + std::to_string(exitY) + ").");
            exit = {exitX, exitY};
            exitCreated = true;
        }
    }

    if (!exitCreated) {
        logger->logMessage("Failed to create a valid exit.");
    }
}

bool Maze::isValidExit(int x, int y) {
    for (const auto& [coordinates, direction] : directionNames) {
        int nx = x + coordinates.first;
        int ny = y + coordinates.second;
        if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1 && mazeGrid[nx][ny] == 1) {
            return false;
        }
    }
    return true;
}

void Maze::carvePassage(int x, int y) {
    std::vector<std::pair<int, int>> directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    mazeGrid[x][y] = 0;
    logger->logMessage("Carving passage at (" + std::to_string(x) + ", " + std::to_string(y) + ").");
    printMazeWithCurrentCarve(x, y);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(directions.begin(), directions.end(), g);

    for (const auto& [dx, dy] : directions) {
        int nx = x + dx * 2;
        int ny = y + dy * 2;

        std::string directionName = directionNames[{dx, dy}];
        logger->logMessage("Trying direction " + directionName + " from (" + std::to_string(x) + ", " + std::to_string(y) + ") to (" + std::to_string(nx) + ", " + std::to_string(ny) + ").");

        if (nx >= 1 && nx < width - 1 && ny >= 1 && ny < height - 1 && mazeGrid[nx][ny] == 1) {
            logger->logMessage("Direction " + directionName + " is valid, moving to (" + std::to_string(nx) + ", " + std::to_string(ny) + ").");
            mazeGrid[x + dx][y + dy] = 0;
            carvePassage(nx, ny);
        } else {
            logger->logMessage("Direction " + directionName + " is invalid or already visited.");
        }
    }

    logger->logMessage("Returning from carving at (" + std::to_string(x) + ", " + std::to_string(y) + ").");
}

void Maze::printMazeWithCurrentCarve(int cx, int cy) const {
    logger->logMessage("Displaying maze with current position:");
    for (int y = height - 1; y >= 0; --y) {
        std::string rowString;
        for (int x = 0; x < width; ++x) {
            if (x == cx && y == cy) {
                rowString += "C ";
            } else {
                rowString += (mazeGrid[x][y] ? '#' : ' ');
                rowString += ' ';
            }
        }
        logger->logMessage(rowString, false);
    }
}
