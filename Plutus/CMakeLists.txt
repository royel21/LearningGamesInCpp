cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
  add_compile_options ("/DP_DEBUG;/DDEBUG")
else()
  add_compile_options ("-DP_DEBUG;-DDEBUG")
endif()

project("Plutus" C CXX)

# Add source files
file(GLOB_RECURSE PLUTUS_SRC src/*.c src/*.cpp)
	
# Add header files
file(GLOB_RECURSE PLUTUS_INC src/*.h src/*.hpp)

include_directories(src)
include_directories(libs/glad/include)
include_directories(libs/glm)
include_directories(libs/Entt)
include_directories(libs/lua/src)
include_directories(libs/stb)
include_directories(libs/sol3/include)
include_directories(libs/spdlog/include)
include_directories(libs/rapidjson/include)
include_directories(libs/box2d/include)
include_directories(libs/glfw/include)
include_directories(libs/freetype/include)

add_subdirectory(libs/glad)
add_subdirectory(libs/lua)
add_subdirectory(libs/stb)
add_subdirectory(libs/spdlog)
add_subdirectory(libs/rapidjson)
add_subdirectory(libs/sol3)
add_subdirectory(libs/box2d)
add_subdirectory(libs/glfw)
add_subdirectory(libs/freetype)

add_library(entt INTERFACE libs/Entt/entt.hpp)

find_package(OpenGL REQUIRED)

set(LIB_DIR ${PROJECT_SOURCE_DIR}/libs/)

# Link the library to the project
link_libraries(glad lua stb spdlog entt rapidjson box2d sol3 glfw freetype opengl32)

add_library(Plutus STATIC ${PLUTUS_SRC} ${PLUTUS_INC})

