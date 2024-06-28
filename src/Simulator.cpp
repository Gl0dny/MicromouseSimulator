#include "Simulator.h"
#include <iostream>
#include <random>
#include <thread>
#include <chrono>

Simulator::Simulator(std::shared_ptr<Micromouse> micromouse, Maze* maze)
    : micromouse(micromouse), maze(maze), startX(1), startY(1), steps(0), running(false), logger(std::make_unique<Logger>("logs/simulator.log")) {
    totalSeconds = std::chrono::duration<double>::zero();
    startTime = std::chrono::steady_clock::time_point::min(); // Initialize with "zero" time
    setRandomStartPosition();
    logger->enableFileOutput(false);
    logger->clearLogFile();
    logger->logMessage("Simulator initialized.");
}

Simulator::~Simulator() {
    logger->disableFileOutput();
}

void Simulator::run() {
    running = true;

    if (startTime == std::chrono::steady_clock::time_point::min()) {
        startTime = std::chrono::steady_clock::now();
    }

    while (running && !hasReachedGoal()) {
        micromouse->move();
        steps = micromouse->getStep();
        displayMazeWithMouse();
        checkAndHandleWallCollision();

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    if (hasReachedGoal()) {
        auto endTime = std::chrono::steady_clock::now();
        totalSeconds = endTime - startTime;
        logger->logMessage("Micromouse escaped from the Maze!");
        logger->logMessage("Total steps taken: " + std::to_string(steps));
        logger->logMessage("Total simulation time: " + std::to_string(totalSeconds.count()) + " seconds");
        exit(0);
    } else {
        logger->logMessage("Simulation paused before the Micromouse could escape.");
    }
}

void Simulator::start() {
    if (!running) {
        std::thread(&Simulator::run, this).detach();
        logger->logMessage("Simulation started.");
    }
}

void Simulator::pause() {
    running = false;
    logger->logMessage("Simulation paused.");
}

void Simulator::reset() {
    if (!running) {
        steps = 0;
        startTime = std::chrono::steady_clock::time_point::min();
        totalSeconds = std::chrono::duration<double>::zero();
        micromouse->reset();
        setRandomStartPosition();
        logger->logMessage("Simulation has been reset.");
    } else {
        logger->logMessage("Cannot reset while the simulation is running. Please pause the simulation first.");
    }
}

void Simulator::setRandomStartPosition() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 3);
    int corner = dis(gen);

    switch (corner) {
        case 0:
            startX = 1;
            startY = 1;
            break;
        case 1:
            startX = maze->getWidth() - 2;
            startY = 1;
            break;
        case 2:
            startX = 1;
            startY = maze->getHeight() - 2;
            break;
        case 3:
            startX = maze->getWidth() - 2;
            startY = maze->getHeight() - 2;
            break;
        default:
            throw std::invalid_argument("Invalid corner specified");
    }
    micromouse->setPosition(startX, startY);
    logger->logMessage("Micromouse starting at (" + std::to_string(startX) + ", " + std::to_string(startY) + ")");
    displayMazeWithMouse();
}

void Simulator::displayMazeWithMouse() const {
    auto grid = micromouse->getKnownMaze();
    int mouseX = micromouse->getPosX();
    int mouseY = micromouse->getPosY();

    for (int y = grid[0].size() - 1; y >= 0; --y) {
        std::string rowString;
        for (int x = 0; x < grid.size(); ++x) {
            if (x == mouseX && y == mouseY) {
                rowString += 'M';
            } else if (grid[x][y] == -1) {
                rowString += '?';
            } else if (grid[x][y] == 1) {
                rowString += '#';
            } else {
                rowString += ' ';
            }
            rowString += ' ';
        }
        logger->logMessage(rowString, false);
    }
    logger->logMessage("Steps taken: " + std::to_string(steps));
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> totalSeconds = currentTime - startTime;
    logger->logMessage("Simulation time: " + std::to_string(totalSeconds.count()) + " seconds");
}

bool Simulator::hasReachedGoal() const {
    auto [exitX, exitY] = maze->readExit();
    return micromouse->getPosX() == exitX && micromouse->getPosY() == exitY;
}

void Simulator::checkAndHandleWallCollision() {
    int currentX = micromouse->getPosX();
    int currentY = micromouse->getPosY();
    if (maze->isWall(currentX, currentY)) {
        logger->logMessage("Collision with wall at (" + std::to_string(currentX) + ", " + std::to_string(currentY) + "). Micromouse died.");
        running = false;
        exit(0);
    }
}
