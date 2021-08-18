cmake_minimum_required(VERSION 3.10)

project(spdlog C CXX)

add_definitions(-DSPDLOG_COMPILED_LIB)

file(GLOB_RECURSE SPDLOG_SRC src/*.c src/*.cpp)
	
# Add header files
file(GLOB_RECURSE SPDLOG_SRC_INC include/*.h include/*.hpp)


add_library(spdlog STATIC ${SPDLOG_SRC} ${SPDLOG_SRC_INC})
