cmake_minimum_required(VERSION 3.15)

project(ZPR-2020Z-atomic-list-test)

message("CXX_FLAGS: " ${CMAKE_CXX_FLAGS})
if("${CMAKE_CXX_FLAGS}" STREQUAL "")
    message("CXX_FLAGS not set")
    set(CMAKE_CXX_FLAGS "-Wall -Werror -Wpedantic -fprofile-arcs -ftest-coverage -fPIC -O0")
    message("CXX_FLAGS: " ${CMAKE_CXX_FLAGS})
endif()

include_directories(${CMAKE_SOURCE_DIR}/googletest)
include_directories(${CMAKE_SOURCE_DIR}/atomic/src)

add_executable(${PROJECT_NAME}
    forward_list_test.cpp
    list_test.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        gmock
        gtest
        gtest_main
)

