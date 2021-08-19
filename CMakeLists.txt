cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("glfwwindow" CXX)
# Add source files
file(GLOB_RECURSE SRC_FILES src/*.c src/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES src/*.h src/*.hpp)

if(MSVC)
  add_compile_options ("/DP_DEBUG;/DDEBUG")
  SET( CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS}
  "/NODEFAULTLIBS:LIBCMT" )
  SET( CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS}
  "/NODEFAULTLIBS:MSVCRT" )
  SET( CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS}
  "/NODEFAULTLIBS:MSVCPRTD" )
else()
  add_compile_options ("-DP_DEBUG;-DDEBUG")
endif()

set(LIB_DIR Plutus/libs)

include_directories(Plutus/src)
include_directories(${LIB_DIR}/glad/include)
include_directories(${LIB_DIR}/glm)
include_directories(${LIB_DIR}/SDL/include)
include_directories(${LIB_DIR}/freetype/include)
include_directories(${LIB_DIR}/Entt)
include_directories(${LIB_DIR}/sol3/include)
include_directories(${LIB_DIR}/lua/src)
include_directories(${LIB_DIR}/spdlog/include)
include_directories(${LIB_DIR}/rapidjson/include)
include_directories(${LIB_DIR}/box2d/include)
include_directories(${LIB_DIR}/freetype/include)

include_directories(PlutusEditor/src)
include_directories(PlutusEditor/libs/imgui/src)

add_subdirectory(Plutus)

find_package(OpenGL REQUIRED)

message("${CMAKE_CXX_COMPILER_ID}")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC Plutus)

# add_executable("TEST" Test/test.cpp Test/Timer.cpp Test/Timer.h)
# target_link_libraries("TEST")

# Editor
add_subdirectory(PlutusEditor)

# Add source files
file(GLOB_RECURSE EDITOR_SRC Editor/*.c Editor/*.cpp)
	
# Add header files
file(GLOB_RECURSE EDITOR_HEADER Editor/*.h Editor/*.hpp)

add_executable("Editor" ${EDITOR_SRC} ${EDITOR_HEADER})
target_link_libraries("Editor" PUBLIC Plutus PlutusEditor)