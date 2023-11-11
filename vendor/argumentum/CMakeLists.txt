cmake_minimum_required(VERSION 3.17)

project(
    vendor_argumentum
    DESCRIPTION "A C++ library for writing command-line program interfaces"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(ARGUMENTUM_BUILD_STATIC_LIBS ON)

include(FetchContent)

# Install argumentum dependency
FetchContent_Declare(Argumentum
   GIT_REPOSITORY https://github.com/mmahnic/argumentum.git
   GIT_TAG v0.3.2
)
FetchContent_MakeAvailable(Argumentum)

# Build a library with argumentum entrypoint
add_library(vendor_argumentum INTERFACE)
target_link_libraries(vendor_argumentum INTERFACE Argumentum::argumentum)
