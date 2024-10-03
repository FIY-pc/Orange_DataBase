cmake_minimum_required(VERSION 3.28)

set(Project_Id ODB_Server)
project(${Project_Id})

include_directories(ORP/include)
include_directories(resources)
add_subdirectory(data)
add_subdirectory(ORP)

add_executable(${Project_Id} src/server.c)
set_target_properties(${Project_Id} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build)