cmake_minimum_required(VERSION 3.5.1)
project(myApp)

set(CMAKE_CXX_STANDARD 14)

find_package(EEROS ${REQUIRED_EEROS_VERSION} REQUIRED)

include_directories(${EEROS_INCLUDE_DIR};${EEROS_LIBUCL_INCLUDE_DIR};${ADDITIONAL_INCLUDE_DIRS})
link_directories(${EEROS_LIB_DIR};${EEROS_LIBUCL_LINK_DIR};${ADDITIONAL_LINK_DIRS})


find_path(BEAGLE_BOARD_BLUE_INCLUDE_DIR NAMES rc_usefulincludes.h roboticscape.h)
if(BEAGLE_BOARD_BLUE_INCLUDE_DIR)
  include_directories(${BEAGLE_BOARD_BLUE_INCLUDE_DIR})
endif()

find_library(BEAGLE_BOARD_BLUE_LIBRARY roboticscape)
if(BEAGLE_BOARD_BLUE_LIBRARY)
  message(STATUS "-> libroboticscape available")
  set(BEAGLE_BOARD_BLUE_LIBRARY_DIR ${BEAGLE_BOARD_BLUE_LIBRARY_DIR} roboticscape)
endif()

add_executable(myApp main.cpp)
target_link_libraries(myApp eeros ${CMAKE_DL_LIBS} ucl roboticscape)
