cmake_minimum_required(VERSION 3.16)
project(cpptrain2)


find_package(Torch REQUIRED)

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

# tests
if(NOT out-test)

    find_package(GTest REQUIRED)

    add_executable(
        ${PROJECT_NAME}-Test 
        ${TEST_SOURCES}
    )

    enable_testing()

    target_include_directories(
        ${PROJECT_NAME}-Test PRIVATE
        ${INCLUDE_DIRECTORIES}
        ${GTEST_INCLUDE_DIRS}
    )
    target_link_libraries(
        ${PROJECT_NAME}-Test PRIVATE
        ${LINK_DIRECTORIES}
        ${TORCH_LIBRARIES}
        GTest::GTest
        GTest::Main
    )
    target_compile_features(
        ${PROJECT_NAME}-Test PRIVATE
        cxx_std_17
    )

    gtest_add_tests(TARGET ${PROJECT_NAME}-Test)

endif()


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
    cxx_std_17
)