
CFLAGS = -I../libs/Entt \
	-I../libs/glm \
	-I../libs/lua/src \
	-I../libs/rapidjson/include \
	-I../libs/sol3/include \
	-I../libs/stb \
	-I../src \
	--memory-init-file 0 -O3 \
	-std=c++1z

CC = em++

BUILD_DIR = ./build

OBJS != find ./ -name "*.o"

SRC_Assets != find ../src/Assets -name *.cpp
SRC_Core != find ../src/Core -name *.cpp
SRC_ECS != find ../src/ECS -name *.cpp
SRC_Graphics != find ../src/Graphics -name *.cpp
SRC_Input != find ../src/Input -name *.cpp
SRC_ParticleSystem != find ../src/ParticleSystem -name *.cpp
SRC_Physics != find ../src/Physics -name *.cpp
SRC_Systems != find ../src/Systems -name *.cpp
SRC_TextRender != find ../src/TextRender -name *.cpp
SRC_Time != find ../src/Time -name *.cpp
SRC_Utils != find ../src/Utils -name *.cpp

OBJ_Assets = $(patsubst %.cpp,%.o, $(notdir ${SRC_Assets}))
OBJ_Core = $(patsubst %.cpp,%.o, $(notdir ${SRC_Core}))
OBJ_ECS = $(patsubst %.cpp,%.o, $(notdir ${SRC_ECS}))
OBJ_Graphics = $(patsubst %.cpp,%.o, $(notdir ${SRC_Graphics}))
OBJ_Input = $(patsubst %.cpp,%.o, $(notdir ${SRC_Input}))
OBJ_ParticleSystem = $(patsubst %.cpp,%.o, $(notdir ${SRC_ParticleSystem}))
OBJ_Physics = $(patsubst %.cpp,%.o, $(notdir ${SRC_Physics}))
OBJ_Systems = $(patsubst %.cpp,%.o, $(notdir ${SRC_Systems}))
OBJ_TextRender = $(patsubst %.cpp,%.o, $(notdir ${SRC_TextRender}))
OBJ_Time = $(patsubst %.cpp,%.o, $(notdir ${SRC_Time}))
OBJ_Utils = $(patsubst %.cpp,%.o, $(notdir ${SRC_Utils}))
















# Targets start here. 
Plutus: Assets Core ECS Graphics Input ParticleSystem Physics Systems TextRender Time Utils
	emcc $(shell find ./ -name "*.o") -r -o../../webbuild/Plutus.o

Assets: ${OBJ_Assets}
	${CC} -c ${SRC_Assets} -I../libs/glm -I../libs/stb -I../src -I../libs/rapidjson/include
Core: ${OBJ_Core}
	${CC} -c ${SRC_Core} ${CFLAGS}
%.o: ../src/ECS/*.cpp ../src/ECS/Components/*.cpp
	${CC} ../src/ECS/*.cpp ../src/ECS/Components/*.cpp -c -o ${CFLAGS}
Graphics: ${OBJ_Graphics}
	${CC} -c ${SRC_Graphics} ${CFLAGS}
Input: 
	${CC} ../src/Input/*.cpp -I../libs/glm -I../src -c 
ParticleSystem: ${OBJ_ParticleSystem}
	${CC} -c ${SRC_ParticleSystem} -I../src -I../libs/glm
Physics: ${OBJ_Physics}
	${CC} -c ${SRC_Physics}  -I../src -I../libs/glm
Systems: ${OBJ_Systems}
	${CC} -c ${SRC_Systems} ${CFLAGS}
TextRender: ${OBJ_TextRender}
	${CC} -c ${SRC_TextRender} -I../libs/freetype/include -I../src -I../libs/glm
Time: ${OBJ_Time}
	${CC} -c ${SRC_Time} -I../src
Utils: ${OBJ_Utils}
	${CC} -c ${SRC_Utils} -I../src -I../libs/rapidjson/include -std=c++1z

clean: 
	rm -rf *.o