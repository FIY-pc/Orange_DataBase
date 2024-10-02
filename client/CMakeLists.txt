cmake_minimum_required(VERSION 3.28)

set(Project_Id ODB_Client)
project(${Project_Id})

add_subdirectory(ORP)
include_directories(ORP/include)
include_directories(resources)

add_executable(${Project_Id} src/client.c)
set_target_properties(${Project_Id} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build)

target_link_libraries(${Project_Id} ORP)