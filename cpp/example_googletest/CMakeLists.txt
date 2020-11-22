cmake_minimum_required(VERSION 3.16)

project(example_googletest)
find_package(GTest REQUIRED)
find_package(Torch REQUIRED)

file(
    GLOB_RECURSE
    TEST_SOURCES
    ${PROJECT_SOURCE_DIR}/*.cpp
)

add_executable(${PROJECT_NAME}-Test ${TEST_SOURCES})

enable_testing()
target_include_directories(
    ${PROJECT_NAME}-Test PUBLIC
    ${GTEST_INCLUDE_DIRS}
)
target_link_libraries(
    ${PROJECT_NAME}-Test PUBLIC
    #${GTEST_LIBRARIES}
    ${TORCH_LIBRARIES}
    GTest::GTest
    GTest::Main
)
target_compile_features(
    ${PROJECT_NAME}-Test PUBLIC
    cxx_std_14
)
gtest_add_tests(TARGET ${PROJECT_NAME}-Test)