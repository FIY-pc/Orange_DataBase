file(GLOB source "src/*.c")

add_library(ORP_Server ${source})
target_include_directories(ORP_Server PRIVATE include)
