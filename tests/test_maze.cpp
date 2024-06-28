#include "Maze.h"
#include "gtest/gtest.h"
#include <set>
#include <string>
#include <memory>

/**
 * @class MazeTest
 * @brief Test fixture for testing the Maze class.
 */
class MazeTest : public ::testing::Test {
protected:
    /**
     * @brief Sets up the test environment.
     */
    void SetUp() override {
        // Setup can be used to initialize variables if needed
    }

    /**
     * @brief Checks if the maze has any 2x2 blocks of walls.
     * @param maze Pointer to the Maze object.
     * @return True if there are 2x2 blocks of walls, false otherwise.
     */
    bool hasAdditionalWalls(const Maze* maze) {
        for (int x = 1; x < maze->getWidth() - 1; ++x) {
            for (int y = 1; y < maze->getHeight() - 1; ++y) {
                if (maze->isWall(x, y) && maze->isWall(x + 1, y) && maze->isWall(x, y + 1) && maze->isWall(x + 1, y + 1)) {
                    return true;
                }
            }
        }
        return false;
    }
};

/**
 * @brief Tests maze generation to ensure there are no 2x2 blocks of walls.
 */
TEST_F(MazeTest, MazeGeneration) {
    Maze* maze = Maze::getInstance();
    
    if (hasAdditionalWalls(maze)) {
        std::cout << "Additional walls found in the maze " << ":\n";
        maze->setLogger("./logs/maze.log", false).displayMaze();
    } else {
        maze->displayMaze();
    }

    EXPECT_FALSE(hasAdditionalWalls(maze));
}

/**
 * @brief Tests the singleton property of the Maze class.
 */
TEST_F(MazeTest, Singleton) {
    Maze* maze1 = Maze::getInstance();
    Maze* maze2 = Maze::getInstance();
    EXPECT_EQ(maze1, maze2);
}

/**
 * @brief Main function to run all tests.
 * @return Result of the test run.
 */
int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
