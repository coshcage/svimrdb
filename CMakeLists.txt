cmake_minimum_required(VERSION 3.31)
project(svimrdb)

include(FetchContent)

FetchContent_Declare(
    stonevalley
    GIT_REPOSITORY https://github.com/coshcage/StoneValley.git
)
FetchContent_MakeAvailable(stonevalley)

add_library(svimrdb)
file(GLOB stonevalley_SRCS LIST_DIRECTORIES false ${stonevalley_SOURCE_DIR}/src/*.c)
target_sources(svimrdb PRIVATE ${stonevalley_SRCS})
target_include_directories(svimrdb PRIVATE ${stonevalley_SOURCE_DIR}/src)
add_subdirectory(src)

