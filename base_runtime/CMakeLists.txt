cmake_minimum_required(VERSION 3.17)

# Define project info
project(
    base_runtime
    DESCRIPTION "Elrond runtime base library"
    LANGUAGES CXX
)

# Define an interface header only library
file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Define a static library
add_library(${PROJECT_NAME}_static STATIC ${SRC_FILES})
target_include_directories(${PROJECT_NAME}_static PUBLIC "${PROJECT_SOURCE_DIR}/include")
set_property(TARGET ${PROJECT_NAME}_static PROPERTY OUTPUT_NAME "elrond_runtime")
target_link_libraries(${PROJECT_NAME}_static PUBLIC Elrond::elrond_runtime)
target_link_libraries(${PROJECT_NAME}_static PRIVATE Elrond::elrond_runtime_src)

# Define a shared library
add_library(${PROJECT_NAME}_shared SHARED ${SRC_FILES})
target_include_directories(${PROJECT_NAME}_shared PUBLIC "${PROJECT_SOURCE_DIR}/include")
set_property(TARGET ${PROJECT_NAME}_shared PROPERTY OUTPUT_NAME "elrond_runtime")
target_link_libraries(${PROJECT_NAME}_shared PUBLIC Elrond::elrond_runtime)
target_link_libraries(${PROJECT_NAME}_shared PRIVATE Elrond::elrond_runtime_src)
