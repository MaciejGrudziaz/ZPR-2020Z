cmake_minimum_required(VERSION 3.15)

project(ZPR-2020Z-test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/googletest)

add_executable(${PROJECT_NAME}
        test.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        gmock
        gtest
        gtest_main
)

