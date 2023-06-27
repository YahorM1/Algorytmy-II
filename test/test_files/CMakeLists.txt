include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(test.cpp)
target_link_libraries(gtest gtest_main)
target_link_libraries(Algorytmy-II)
