cmake_minimum_required(VERSION 3.16.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options ("/DP_DEBUG;")

project("PlutusEditor" C CXX)


set(LIBS ${CMAKE_SOURCE_DIR}/Plutus/libs)
# Add source files
file(GLOB_RECURSE EDITOR_SRC src/*.c src/*.cpp src/*.h src/*.hpp)

add_compile_options ("/DIMGUI_IMPL_OPENGL_LOADER_GLAD;/DP_DEBUG;")

include_directories(${CMAKE_SOURCE_DIR}/Plutus/src)
include_directories(${LIBS}/)
include_directories(${LIBS}/glad/include)
include_directories(${LIBS}/SDL/include)
include_directories(${LIBS}/glm)
include_directories(${LIBS}/Entt)
include_directories(${LIBS}/glfw/include)
include_directories(src/)
include_directories(libs/imgui/src)

add_subdirectory(libs/imgui)

# Link the library to the project
link_libraries(imgui)

add_library(PlutusEditor STATIC ${EDITOR_SRC})