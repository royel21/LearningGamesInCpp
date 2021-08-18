cmake_minimum_required(VERSION 3.10)

project(glad C CXX)

file(GLOB_RECURSE GLAD_SRC src/*.c src/*.cpp)
	
# Add header files
file(GLOB_RECURSE GLAD_SRC_INC include/*.h include/*.hpp)

add_library(glad STATIC ${GLAD_SRC} ${GLAD_SRC_INC})
