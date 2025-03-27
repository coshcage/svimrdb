file(GLOB svimrdb_SRCS LIST_DIRECTORIES false *.c)
target_include_directories(svimrdb PUBLIC .)
target_sources(svimrdb PUBLIC ${svimrdb_SRCS})

