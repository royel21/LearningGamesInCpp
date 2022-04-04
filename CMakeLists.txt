cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(game C CXX)

file(GLOB_RECURSE GAME_SRC Projects/game/*.h Projects/game/*.cpp)

add_subdirectory(Plutus)

# Add Game Target
add_executable(game ${GAME_SRC})
target_link_libraries(game Plutus)

# ------------------------Plutus Editor-----------------------------------#
set(PE_DIR Projects/PlutusEdit/src)
file(GLOB_RECURSE PE_SRC ${PE_DIR}/*.h ${PE_DIR}/*.cpp)

set(IMGUI_DIR Projects/PlutusEdit/libs/imgui/src)

file(GLOB_RECURSE IMGUI_SRC 
    ${IMGUI_DIR}/*.c 
    ${IMGUI_DIR}/*.cpp 
    ${IMGUI_DIR}/*.h 
    ${IMGUI_DIR}/*.hpp
)

# include Plutus Edit Headers
include_directories(${PE_DIR})
include_directories(${IMGUI_DIR})

# Add PlutusEdit Target
add_executable("PlutusEdit" ${PE_SRC} ${IMGUI_SRC})
target_link_libraries("PlutusEdit" PUBLIC Plutus)
# ***************************End Plutus Editor********************************#


#**************Playground***********************************#
# play ground
file(GLOB_RECURSE P_Ground Projects/playground/*.h Projects/playground/*.cpp)

add_executable("playground" ${P_Ground})
target_link_libraries("playground" PUBLIC Plutus)

#***************TestingBox2d*************************

file(GLOB TEST_SRC Projects/TestingBox2d/*.cpp Projects/TestingBox2d/*.h)

add_executable(TestingBox2d ${TEST_SRC})
target_link_libraries(TestingBox2d PUBLIC Plutus)

#*************Single Test**********************
add_executable("SingleTest" Projects/SingleTest.cpp)
target_link_libraries(SingleTest PUBLIC Plutus)