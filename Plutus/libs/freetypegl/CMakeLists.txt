
cmake_minimum_required(VERSION 3.10)

project(freetypegl C CXX)

file(GLOB_RECURSE FILES_SRC ./*.c ./*.h)
	
# Add header files

add_library(freetypegl STATIC ${FILES_SRC})