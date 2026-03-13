cmake_minimum_required(VERSION 3.10)
project(CCompiler VERSION 1.0)

# Enforce modern C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Tell CMake where to find your header files
include_directories(include)

# Group your source files
set(SOURCES
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/codegen.cpp
)

# Create the executable
add_executable(ccompiler ${SOURCES})

# Copy the tests directory to the build output directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tests DESTINATION ${CMAKE_CURRENT_BINARY_DIR})