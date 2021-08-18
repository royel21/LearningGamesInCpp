
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

# Targets start here. 
Plutus: Assets Core ECS Graphics Input ParticleSystem Physics Systems TextRender Time Utils
	emcc $(shell find ./ -name "*.o") -r -o../../webbuild/Plutus.o

Assets: ${SRC_Assets}
	${CC} -c ${SRC_Assets} -I../libs/glm -I../libs/stb -I../src -I../libs/rapidjson/include
Core: ${SRC_Core}
	${CC} -c ${SRC_Core} ${CFLAGS}
ECS: ${SRC_ECS}
	${CC} -c ${SRC_ECS} ${CFLAGS}
Graphics: ${SRC_Graphics}
	${CC} -c ${SRC_Graphics} ${CFLAGS}
Input: ${SRC_Input}
	${CC} -c ${SRC_Input} -I../libs/glm -I../src
ParticleSystem: ${SRC_ParticleSystem}
	${CC} -c ${SRC_ParticleSystem} -I../src -I../libs/glm
Physics: ${SRC_Physics}
	${CC} -c ${SRC_Physics}  -I../src -I../libs/glm
Systems: ${SRC_Systems}
	${CC} -c ${SRC_Systems} ${CFLAGS}
TextRender: ${SRC_TextRender}
	${CC} -c ${SRC_TextRender} -I../libs/freetype/include -I../src -I../libs/glm
Time: ${SRC_Time}
	${CC} -c ${SRC_Time} -I../src
Utils: ${SRC_Utils}
	${CC} -c ${SRC_Utils} -I../src -I../libs/rapidjson/include -std=c++1z

clean: 
	rm -rf *.o