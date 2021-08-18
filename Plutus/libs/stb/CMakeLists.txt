cmake_minimum_required(VERSION 3.10)

project("stb" C CXX)

file(GLOB_RECURSE STB_SRC ${PROJECT_SOURCE_DIR}/*.c)
	
# Add header files
file(GLOB_RECURSE STB_INC ${PROJECT_SOURCE_DIR}/*.h)

add_library(stb STATIC ${STB_SRC} ${STB_INC})

