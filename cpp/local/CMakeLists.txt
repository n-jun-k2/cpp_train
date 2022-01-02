cmake_minimum_required(VERSION 3.16)
project(local)


find_package(Torch REQUIRED)

set(cpp_version cxx_std_17)

# Set macros
add_definitions(
)

# project
file(
    GLOB_RECURSE
    SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/include/*.cpp
)

# test
file(
    GLOB_RECURSE
    TEST_SOURCES
    ${PROJECT_SOURCE_DIR}/test/*.cpp
    ${PROJECT_SOURCE_DIR}/include/*.cpp
)

# libs
include_directories(
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/include
)
link_directories(
    ${PROJECT_SOURCE_DIR}/libs
)

add_executable(
    ${PROJECT_NAME} 
    ${SOURCES}
)
target_link_libraries(
    ${PROJECT_NAME} 
    ${LINK_DIRECTORIES}
    ${TORCH_LIBRARIES}
)
target_compile_features(
    ${PROJECT_NAME} PRIVATE
    ${cpp_version}
)