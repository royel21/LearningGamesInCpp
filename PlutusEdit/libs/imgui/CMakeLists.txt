
cmake_minimum_required(VERSION 3.16.2)

project(imgui C CXX)

# include_directories(${CMAKE_SOURCE_DIR}/Plutus/libs/glm)

file(GLOB_RECURSE IMGUI_SRC src/*.c src/*.cpp src/*.h src/*.hpp)

add_library(imgui STATIC ${IMGUI_SRC})
