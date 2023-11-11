cmake_minimum_required(VERSION 3.17)
enable_testing()

# Define project info
project(
    elrond_runtime
    VERSION 0.1.0
    DESCRIPTION "Elrond Runtime Application"
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
set(ELROND_COMMON_BASE_DIR "${PROJECT_SOURCE_DIR}/elrond/")
if(DEFINED ELROND_COMMON)
    set(ELROND_COMMON_BASE_DIR "${ELROND_COMMON}")
endif()

if(NOT EXISTS "${ELROND_COMMON_BASE_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "No valid Elrond Common Library path was found.")
endif()

message(STATUS "Using \"${ELROND_COMMON_BASE_DIR}\" as Elrond Common Library")

# Check if the Elrond Common Library was configured
if(NOT EXISTS "${ELROND_COMMON_BASE_DIR}/build/elrond.cmake")
    message(FATAL_ERROR "The Elrond Common Library was not been configured or build (missing \"build/elrond.cmake\" file).")
endif()

# Include Elrond Common Library exports
include("${ELROND_COMMON_BASE_DIR}/build/elrond.cmake")
message(STATUS "Including \"${ELROND_COMMON_BASE_DIR}/build/elrond.cmake\" from Elrond Common Library")

# Add subprojects
add_subdirectory(vendor/argumentum)
add_subdirectory(vendor/nlohmann_json)
add_subdirectory(runtime)
add_subdirectory(elrond_cli)
