cmake_minimum_required(VERSION 3.17)

project(
    dlobject
    DESCRIPTION "Misc runtime tests"
    LANGUAGES CXX
)

# Copy External example module from common project tests
file(GLOB EXT_MODULE_FILE "${ELROND_COMMON_BASE_DIR}/build/test/dlobject/ExternalModule.*")
file(
    COPY ${EXT_MODULE_FILE}
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)
