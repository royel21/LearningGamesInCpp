cmake_minimum_required(VERSION 3.10)

project("rapidjson" C CXX)

file(GLOB_RECURSE RAPIDJSON_SRC include/*.h include/*.hpp)
	
add_library(rapidjson INTERFACE ${RAPIDJSON_SRC})

