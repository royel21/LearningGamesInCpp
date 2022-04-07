
# Add source files
file(GLOB_RECURSE ASSETS src/Assets/*.h src/Assets/*.cpp)
file(GLOB CORE src/Core/*.h src/Core/*.cpp)
file(GLOB ECS src/ECS/*.h src/ECS/*.cpp src/ECS/Components/*.h src/ECS/Components/*.cpp)
file(GLOB GRAPHICS src/Graphics/*.h src/Graphics/*.cpp)
file(GLOB INPUT src/Input/*.h src/Input/*.cpp)
file(GLOB PARTICLES src/Particles/*.h src/Particles/*.cpp)
file(GLOB SERIALIZE src/Serialize/*.h src/Serialize/*.cpp)
file(GLOB SYSTEMS src/Systems/*.h src/Systems/System/*.h src/Systems/*.cpp src/Systems/System/*.cpp)
file(GLOB PHYSICS src/Physics/*.h src/Physics/*.cpp)
file(GLOB TIME src/Time/*.h src/Time/*.cpp)
file(GLOB UTILS src/Utils/*.h src/Utils/*.cpp)
file(GLOB MATH src/Math/*.h src/Math/*.cpp)
file(GLOB LOG src/Log/*.h src/Log/*.cpp)
set(BASE_SRCS ${ASSETS} ${CORE} ${ECS} ${GRAPHICS} ${INPUT} ${PARTICLES} ${SERIALIZE} ${SYSTEMS} ${TIME} ${UTILS} ${PHYSICS} ${MATH} ${LOG} src/Core.h src/Core.cpp)

file(GLOB PLATFORMS_WIN src/Platforms/Windows/*.h src/Platforms/Windows/*.cpp)
file(GLOB PLATFORMS_ANDROID src/Platforms/Android/*.h src/Platforms/Android/*.cpp)

if (WIN32)
  list(APPEND BASE_SRCS ${PLATFORMS_WIN})
elseif (ANDROID )
  list(APPEND BASE_SRCS ${PLATFORMS_ANDROID})
endif ()

add_subdirectory(libs)

find_package(OpenGL REQUIRED)

# Link the library to the project
link_libraries(glad lua stb rapidjson box2d sol3 glfw freetype opengl32)

add_library(Plutus STATIC ${BASE_SRCS})

target_include_directories(Plutus
    PUBLIC src
    PUBLIC libs/box2d/include
    PUBLIC libs/glad/include
    PUBLIC libs/glm
    PUBLIC libs/Entt
    PUBLIC libs/lua/src
    PUBLIC libs/stb
    PUBLIC libs/sol3/include
    PUBLIC libs/rapidjson/include
    PUBLIC libs/glfw/include
    PUBLIC libs/freetype/include
    PUBLIC libs/spdlog/include
)
