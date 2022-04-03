
cmake_minimum_required(VERSION 3.10)

project(box2d C CXX)

file(GLOB_RECURSE BOX2D_SRC src/*.c src/*.cpp)
	
# # Add header files
# file(GLOB_RECURSE BOX2D_SRC_INC include/*.h include/*.hpp)

include_directories(include)

add_library(box2d STATIC ${BOX2D_SRC})