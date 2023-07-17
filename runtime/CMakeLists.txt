cmake_minimum_required(VERSION 3.17)

# Define project info
project(
    runtime
    DESCRIPTION "Elrond Runtime Library"
    LANGUAGES CXX
)

# Look for all source code files
file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Find threads library
find_package(Threads REQUIRED)

# Define a static library
add_library(${PROJECT_NAME}_static STATIC ${SRC_FILES})
target_include_directories(${PROJECT_NAME}_static PUBLIC "${PROJECT_SOURCE_DIR}/include")
set_property(TARGET ${PROJECT_NAME}_static PROPERTY OUTPUT_NAME "elrond_runtime")
target_link_libraries(${PROJECT_NAME}_static PUBLIC Elrond::runtime Threads::Threads)
target_link_libraries(${PROJECT_NAME}_static PRIVATE Elrond::runtime_src)

# Define a shared library
add_library(${PROJECT_NAME}_shared SHARED ${SRC_FILES})
target_include_directories(${PROJECT_NAME}_shared PUBLIC "${PROJECT_SOURCE_DIR}/include")
set_property(TARGET ${PROJECT_NAME}_shared PROPERTY OUTPUT_NAME "elrond_runtime")
target_link_libraries(${PROJECT_NAME}_shared PUBLIC Elrond::runtime Threads::Threads)
target_link_libraries(${PROJECT_NAME}_shared PRIVATE Elrond::runtime_src)

add_subdirectory(test)
