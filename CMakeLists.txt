cmake_minimum_required(VERSION 3.15)

project(ZPR-2020Z)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-Wall -Werror -Wpedantic -fprofile-arcs -ftest-coverage -fPIC -O0")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BOOST_ENABLE_CMAKE ON)

add_subdirectory(boost)
add_subdirectory(googletest)
add_subdirectory(atomic)

include_directories(
        boost/libs
)
