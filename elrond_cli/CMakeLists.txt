cmake_minimum_required(VERSION 3.17)

# Define project info
project(
    elrond_cli
    DESCRIPTION "Elrond CLI Application"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

# Look for all source code files
file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Find threads library
find_package(Threads REQUIRED)

# Define a static library
add_executable(elrond ${SRC_FILES})
target_include_directories(elrond PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(elrond PUBLIC runtime_static Threads::Threads vendor_argumentum)
