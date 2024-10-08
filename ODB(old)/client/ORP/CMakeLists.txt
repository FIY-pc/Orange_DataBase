file(GLOB source "src/*.c")

add_library(ORP ${source})
target_include_directories(ORP PUBLIC include)
