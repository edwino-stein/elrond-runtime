cmake_minimum_required(VERSION 3.17)

project(
    vendor_nlohmann_json
    DESCRIPTION "JSON for Modern C++"
    LANGUAGES CXX
)

include(FetchContent)

# Install argumentum dependency
FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

add_library(vendor_nlohmann_json INTERFACE)
target_link_libraries(vendor_nlohmann_json INTERFACE nlohmann_json::nlohmann_json)
