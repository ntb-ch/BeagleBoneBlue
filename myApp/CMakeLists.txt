cmake_minimum_required(VERSION 2.8)
project(myApp)

find_package(EEROS REQUIRED)
include_directories(${EEROS_INCLUDE_DIR};${EEROS_LIBUCL_INCLUDE_DIR})
link_directories(${EEROS_LIB_DIR};${EEROS_LIBUCL_LINK_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# find_library(BEAGLE_BOARD_BLUE_LIBRARY roboticscape)
link_directories("~/Projects/BBB/Robotics_Cape_Installer/libraries/")

add_executable(myApp main.cpp)
target_link_libraries(myApp eeros ${CMAKE_DL_LIBS} ucl roboticscape)

