#!/bin/bash

# Function to check if CMake is installed
check_cmake() {
    if ! command -v cmake &> /dev/null
    then
        echo "CMake is not installed. Please install CMake and try again."
        exit 1
    fi
}

# Function to check if Qt5 is installed
check_qt() {
    if ! command -v qmake &> /dev/null
    then
        echo "Qt5 is not installed. Please install Qt5 and try again."
        exit 1
    fi
}

# Function to check if Doxygen is installed
check_doxygen() {
    if ! command -v doxygen &> /dev/null
    then
        echo "Doxygen is not installed. Please install Doxygen and try again."
        exit 1
    fi
}

# Function to display help
show_help() {
    echo "Usage: ./build.sh [options]"
    echo "Options:"
    echo "  --help     Displays this help message"
    echo "  --test     Builds the project and runs tests only, without running the main application"
    echo "  --clean    Cleans the build directory and all output files"
    echo "  --debug    Builds the project in debug mode"
    echo "  --doc      Generates Doxygen documentation"
}

# Function to clean the build directory and output files
clean_build() {
    echo "Cleaning the build directory and all output files..."
    rm -rf build
    rm -rf logs
    rm -rf tests/logs
    if [ $? -ne 0 ]; then
        echo "Cleaning failed."
        exit 1
    fi
    echo "Cleaning completed successfully."
}

# Function to build the project
build() {
    echo "Checking if cmake is installed..."
    check_cmake

    # echo "Checking if QT is installed..."
    # check_qt

    echo "Creating build directory if it does not exist..."
    if [ ! -d "build" ]; then
        mkdir build
    fi

    cd build || exit

    # Configure the project using CMake
    echo "Configuring the project using CMake..."
    if $DEBUG; then
        cmake -DCMAKE_BUILD_TYPE=Debug ..
    else
        cmake ..
    fi

    # Check if configuration was successful
    if [ $? -ne 0 ]; then
        echo "CMake configuration failed."
        exit 1
    fi

    # Compile the project
    echo "Compiling the project..."
    make

    # Check if compilation was successful
    if [ $? -ne 0 ]; then
        echo "Project compilation failed."
        exit 1
    fi

    cd ..
}

# Function to generate Doxygen documentation
generate_docs() {
    check_doxygen
    echo "Generating Doxygen documentation..."

    # Check if ./docs/Doxyfile exists
    if [ ! -f "./docs/Doxyfile" ]; then
        # Create the docs directory if it does not exist
        mkdir -p docs

        # Generate Doxyfile inside the docs directory
        doxygen -g docs/Doxyfile
    fi

    # Generate documentation using the Doxyfile in the docs directory
    doxygen docs/Doxyfile

    if [ $? -ne 0 ]; then
        echo "Doxygen documentation generation failed."
        exit 1
    fi
    echo "Doxygen documentation generated successfully."
}

# Function to run the application
run_application() {
    echo "Running the application..."
    ./build/MicromouseProject

    # Check if the application ran successfully
    if [ $? -ne 0 ]; then
        echo "Application execution failed."
        exit 1
    fi
}

# Function to run tests
run_tests() {
    echo "Running tests..."
    ./build/tests

    # Check if tests ran successfully
    if [ $? -ne 0 ]; then
        echo "Tests failed."
        exit 1
    fi
    cd ..
}

# Flags
CLEAN=false
TEST=false
DEBUG=false
DOC=false

# Parse arguments
for arg in "$@"
do
    case $arg in
        --clean)
        CLEAN=true
        shift # Remove argument
        ;;
        --test)
        TEST=true
        shift # Remove argument
        ;;
        --debug)
        DEBUG=true
        shift # Remove argument
        ;;
        --doc)
        DOC=true
        shift # Remove argument
        ;;
        --help)
        show_help
        exit 0
        ;;
        *)
        echo "Unknown argument: $arg"
        show_help
        exit 1
        ;;
    esac
done

if $DOC; then
    generate_docs
    exit 0
fi

# Perform appropriate actions based on flags
if $CLEAN; then
    clean_build
fi

if $TEST; then
    build
    run_tests
else
    build
    run_application
fi
