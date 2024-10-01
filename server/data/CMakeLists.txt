file(GLOB source "src/*.c")

add_library(data ${source})
target_include_directories(data PRIVATE include)