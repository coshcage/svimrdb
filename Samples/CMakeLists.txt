
FetchContent_Declare(
    svregex
    GIT_REPOSITORY https://github.com/coshcage/svregex.git
    GIT_TAG a16a78016c20edfe472e2dbdae97cc5d1b045b9e
)
FetchContent_MakeAvailable(svregex)

function(use_svregex target)
    set(svregex_SRCS ${svregex_SOURCE_DIR}/src/svregex.c)
    target_sources(${target} PRIVATE ${svregex_SRCS})
    target_include_directories(${target} PRIVATE ${svregex_SOURCE_DIR}/src)
endfunction()


#exp_2023-07-08_1.c
add_executable(sample20230708 exp_2023-07-08_1.c)
target_link_libraries(sample20230708 svimrdb)
use_stonevalley(sample20230708)

#exp_2023-08-09_1.c
add_executable(sample20230809 exp_2023-08-09_1.c)
target_link_libraries(sample20230809 svimrdb)
use_stonevalley(sample20230809)

#exp_2024-01-30_1.c
add_executable(sample20240130 exp_2024-01-30_1.c)
target_link_libraries(sample20240130 svimrdb)
use_stonevalley(sample20240130)

#exp_2024-04-18_1.c
add_executable(sample20240418 exp_2024-04-18_1.c)
target_link_libraries(sample20240418 svimrdb)
use_stonevalley(sample20240418)
use_svregex(sample20240418)
