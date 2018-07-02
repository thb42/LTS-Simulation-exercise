cmake_minimum_required(VERSION 2.8)
project(LTS-Simulation)

#set directories
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories(${PROJECT_SOURCE_DIR}/headers)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)
