cmake_minimum_required(VERSION 3.5.1)

project(demo)

set(CMAKE_CXX_STANDARD 14)


set(RECOMMENDED_EEROS_VERSION 1.0.0.0)
set(RECOMMENDED_BBB_EEROS_VERSION 1.1.0.0)


if(CMAKE_BUILD_TYPE MATCHES "Debug|DEBUG")
	set(LOGGER ON)
endif()


include(cmake/versionCheckHelpers.cmake)

# Find EEROS
setVersionToFind("EEROS" "REQUIRED_EEROS_VERSION" ${RECOMMENDED_EEROS_VERSION} ${REQUIRED_EEROS_VERSION})
find_package(EEROS ${VERSION_TO_FIND} EXACT)
checkIfModuleFound("EEROS" "REQUIRED_EEROS_VERSION")


# Find bbb-eeros
setVersionToFind("bbb-eeros" "REQUIRED_BBB_EEROS_VERSION" ${RECOMMENDED_BBB_EEROS_VERSION} ${REQUIRED_BBB_EEROS_VERSION})
find_package(bbb-eeros ${VERSION_TO_FIND} EXACT)
checkIfModuleFound("bbb-eeros" "REQUIRED_BBB_EEROS_VERSION")


include_directories(${EEROS_INCLUDE_DIR};${EEROS_LIBUCL_INCLUDE_DIR};${ADDITIONAL_INCLUDE_DIRS})
link_directories(${EEROS_LIB_DIR};${EEROS_LIBUCL_LINK_DIR};${ADDITIONAL_LINK_DIRS})

add_executable(demo main.cpp)
target_link_libraries(demo eeros ${CMAKE_DL_LIBS} ucl)

