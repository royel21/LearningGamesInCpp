
# Add source files
file(GLOB_RECURSE BASE_SRCS src/*.h src/*.cpp)
file(GLOB PLATFORMS_WIN Platforms/Windows/*.h Platforms/Windows/*.cpp)
file(GLOB PLATFORMS_ANDROID Platforms/Android/*.h Platforms/Android/*.cpp)

if (WIN32)
  list(APPEND BASE_SRCS ${PLATFORMS_WIN})
elseif (ANDROID )
  list(APPEND BASE_SRCS ${PLATFORMS_ANDROID})
endif ()

add_subdirectory(libs)
add_library(entt INTERFACE libs/Entt/entt.hpp)

find_package(OpenGL REQUIRED)

# Link the library to the project
link_libraries(glad lua stb entt rapidjson box2d sol3 glfw freetype opengl32)

add_library(Plutus STATIC ${BASE_SRCS})

target_include_directories(Plutus
    PUBLIC src
    PUBLIC Platforms
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
)