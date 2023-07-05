cmake_minimum_required(VERSION 3.22)
project(Algorytmy-II)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(test/googletest)
include_directories(test/googletest/googletest/include)
include_directories(test/googletest/googlemock/include)

add_executable(runTests test/test_files/test.cpp)
target_link_libraries(runTests gtest gtest_main)

add_executable(Algorytmy-II brut-force.cpp)
#add_executable(Algorytmy-II Presentation.cpp HungarianClass/hungarian.cpp  HungarianClass/hungarian.h  HungarianClass/types.h)
# add_executable(Algorytmy-II test/test_files/test.cpp)