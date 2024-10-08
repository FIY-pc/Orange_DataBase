cmake_minimum_required(VERSION 3.28)

set(Project_Id ODB-Client)
project(${Project_Id})

add_subdirectory(ORP)
include_directories(ORP/include)
include_directories(resources)
include_directories(include)

file(GLOB source "src/*.c")
add_executable(${Project_Id} ${source})
set_target_properties(${Project_Id} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

target_link_libraries(${Project_Id} ORP)