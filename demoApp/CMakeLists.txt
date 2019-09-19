cmake_minimum_required(VERSION 3.5.1)
project(demo)

set(CMAKE_CXX_STANDARD 14)

set(RECOMMENDED_EEROS_VERSION 1.0.0.0)


if(NOT DEFINED REQUIRED_EEROS_VERSION OR
   NOT REQUIRED_EEROS_VERSION MATCHES
       "^(0|[1-9][0-9]*)[.](0|[1-9][0-9]*)[.](0|[1-9][0-9]*)[.](0|[1-9][0-9]*)$")

  set(REQUIRED_EEROS_VERSION ${RECOMMENDED_EEROS_VERSION})
  message("Using recommended EEROS version, which is: v${REQUIRED_EEROS_VERSION}")

else()
  if(NOT ${REQUIRED_EEROS_VERSION} EQUAL ${RECOMMENDED_EEROS_VERSION})
     message("You are not using the recommended EEROS version, "
             "which would be: v${RECOMMENDED_EEROS_VERSION}")
  endif(NOT ${REQUIRED_EEROS_VERSION} EQUAL ${RECOMMENDED_EEROS_VERSION})
endif()


find_package(EEROS ${REQUIRED_EEROS_VERSION} REQUIRED)

include_directories(${EEROS_INCLUDE_DIR};${EEROS_LIBUCL_INCLUDE_DIR};${ADDITIONAL_INCLUDE_DIRS})
link_directories(${EEROS_LIB_DIR};${EEROS_LIBUCL_LINK_DIR};${ADDITIONAL_LINK_DIRS})

add_executable(demo main.cpp)
target_link_libraries(demo eeros ${CMAKE_DL_LIBS} ucl)
