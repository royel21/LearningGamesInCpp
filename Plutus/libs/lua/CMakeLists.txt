cmake_minimum_required(VERSION 3.10)

project("lua" C CXX)

file(GLOB_RECURSE LUA_SRC 
	${PROJECT_SOURCE_DIR}/src/*.c
	${PROJECT_SOURCE_DIR}/src/*.cpp)
	
# Add header files
file(GLOB_RECURSE LUA_INC
	${PROJECT_SOURCE_DIR}/src/*.h
	${PROJECT_SOURCE_DIR}/src/*.hpp)

include_directories(${PROJECT_SOURCE_DIR}/src/)


add_library(lua STATIC ${LUA_SRC} ${LUA_INC})

