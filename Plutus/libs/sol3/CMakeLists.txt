cmake_minimum_required(VERSION 3.10)

project("sol3" C CXX)

file(GLOB_RECURSE SOL3_SRC include/*.h include/*.hpp)
	
add_library(sol3 INTERFACE ${SOL3_SRC})

