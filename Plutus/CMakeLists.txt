cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("Plutus" C CXX)

# Add source files
file(GLOB ASSETS src/Assets/*.h src/Assets/*.cpp)
file(GLOB CORE src/Core/*.h src/Core/*.cpp)
file(GLOB ECS src/ECS/*.h src/ECS/*.cpp src/ECS/Components/*.h src/ECS/Components/*.cpp)
file(GLOB GRAPHICS src/Graphics/*.h src/Graphics/*.cpp)
file(GLOB INPUT src/Input/*.h src/Input/*.cpp)
file(GLOB PARTICLES src/Particles/*.h src/Particles/*.cpp)
file(GLOB SERIALIZE src/Serialize/*.h src/Serialize/*.cpp)
file(GLOB SYSTEMS src/Systems/*.h src/Systems/*.cpp)
file(GLOB PHYSICS src/Physics/*.h src/Physics/*.cpp)
file(GLOB TIME src/Time/*.h src/Time/*.cpp)
file(GLOB UTILS src/Utils/*.h src/Utils/*.cpp)
set(BASE_SRCS ${ASSETS} ${CORE} ${ECS} ${GRAPHICS} ${INPUT} ${PARTICLES} ${SERIALIZE} ${SYSTEMS} ${TIME} ${UTILS} ${PHYSICS})

file(GLOB PLATFORMS_WIN src/Platforms/Windows/*.h src/Platforms/Windows/*.cpp)
file(GLOB PLATFORMS_ANDROID src/Platforms/Android/*.h src/Platforms/Android/*.cpp)

if (WIN32)
  list(APPEND BASE_SRCS ${PLATFORMS_WIN})
elseif (ANDROID )
  list(APPEND BASE_SRCS ${PLATFORMS_ANDROID})
endif ()

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

add_library(Plutus STATIC ${BASE_SRCS})

