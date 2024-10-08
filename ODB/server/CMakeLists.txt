cmake_minimum_required(VERSION 3.28)

set(Project_Id ODB-Server)
project(${Project_Id})

include_directories(ORP/include)
include_directories(data/include)
include_directories(include)
add_subdirectory(data)
add_subdirectory(ORP)

file(GLOB source "src/*.c")
add_executable(${Project_Id} ${source})
set_target_properties(${Project_Id} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
target_link_libraries(${Project_Id} ORP)
target_link_libraries(${Project_Id} data)