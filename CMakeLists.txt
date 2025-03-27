cmake_minimum_required(VERSION 3.31)
project(svimrdb)

option(BUILD_SAMPLES "Build files in Samples folder. " OFF)

include(FetchContent)

FetchContent_Declare(
    stonevalley
    GIT_REPOSITORY https://github.com/coshcage/StoneValley.git
)
FetchContent_MakeAvailable(stonevalley)

function(use_stonevalley target)
    file(GLOB stonevalley_SRCS LIST_DIRECTORIES false ${stonevalley_SOURCE_DIR}/src/*.c)
    target_sources(${target} PRIVATE ${stonevalley_SRCS})
    target_include_directories(${target} PRIVATE ${stonevalley_SOURCE_DIR}/src)
endfunction()

add_library(svimrdb)
use_stonevalley(svimrdb)
add_subdirectory(src)

if(BUILD_SAMPLES)
    add_subdirectory(Samples)
endif()

