cmake_minimum_required(VERSION 3.17)
enable_testing()

# Define project info
project(
    elrond_runtime
    VERSION 0.1.0
    DESCRIPTION "Elrond runtime application"
    LANGUAGES CXX
)

# Check the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Define project build stage
set(elrond_DEV_STAGE Alpha)
set(elrond_BUILD_TYPE ${CMAKE_BUILD_TYPE})

# Define C++11 as minimum requirement
set(CMAKE_CXX_STANDARD 11)

# Define warning level
if(MSVC)
    string(REGEX REPLACE "/W[1-3]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
    endif()
else()
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-Wall -Wextra -pedantic -Werror)
    else()
        add_compile_options(-Wall -Wextra)
    endif()
endif()

# Look for and define Elrond Common Library path
set(ELROND_COMMON_DIR "${PROJECT_SOURCE_DIR}/elrond")
if (EXISTS "${ELROND_COMMON_DIR}-dev" AND EXISTS "${ELROND_COMMON_DIR}-dev/CMakeLists.txt")
    set(ELROND_COMMON_DIR "${ELROND_COMMON_DIR}-dev")
else()
    if (NOT EXISTS "${ELROND_COMMON_DIR}" OR NOT EXISTS "${ELROND_COMMON_DIR}/CMakeLists.txt")
        message(FATAL_ERROR "No Elrond Common Library found")
    endif()
endif()

message(STATUS "Using \"${ELROND_COMMON_DIR}\" as Elrond Common Library")

# Include Elrond Common Library exports
include("${ELROND_COMMON_DIR}/build/elrond.cmake")

# Find threads library
find_package(Threads REQUIRED)

# Add subprojects
add_subdirectory(base_runtime)
add_subdirectory(test)
