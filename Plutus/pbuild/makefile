
CFLAGS = -I../libs/Entt \
	-I../libs/lua/src \
	-I../libs/rapidjson/include \
	-I../libs/sol3/include \
	-I../libs/stb \
	-I../src \
	-I../libs/freetype/include \
	-I../libs/box2d/include \
	--memory-init-file 0 -O3 \
	-std=c++1z

CC = em++

BUILD_DIR = ./Build

OBJS != find ./ -name "*.o"

SRC_Assets != find ../src/Assets -name *.cpp
SRC_Core != find ../src/Core -name *.cpp
SRC_ECS != find ../src/ECS -name *.cpp
SRC_Graphics != find ../src/Graphics -name *.cpp
SRC_Input != find ../src/Input -name *.cpp
SRC_ParticleSystem != find ../src/ParticleSystem -name *.cpp
SRC_Physics != find ../src/Physics -name *.cpp
SRC_Systems != find ../src/Systems -name *.cpp
SRC_Time != find ../src/Time -name *.cpp
SRC_Utils != find ../src/Utils -name *.cpp
SRC_Platforms != find ../src/Platforms/Web -name *.cpp
SRC_Math != find ../src/Math -name *.cpp
SRC_Serialize != find ../src/Serialize -name *.cpp

OBJ_Assets = $(patsubst %.cpp,%.o, $(notdir ${SRC_Assets}))
OBJ_Core = $(patsubst %.cpp,%.o, $(notdir ${SRC_Core}))
OBJ_ECS = $(patsubst %.cpp,%.o, $(notdir ${SRC_ECS}))
OBJ_Graphics = $(patsubst %.cpp,%.o, $(notdir ${SRC_Graphics}))
OBJ_Input = $(patsubst %.cpp,%.o, $(notdir ${SRC_Input}))
OBJ_ParticleSystem = $(patsubst %.cpp,%.o, $(notdir ${SRC_ParticleSystem}))
OBJ_Physics = $(patsubst %.cpp,%.o, $(notdir ${SRC_Physics}))
OBJ_Systems = $(patsubst %.cpp,%.o, $(notdir ${SRC_Systems}))
OBJ_Time = $(patsubst %.cpp,%.o, $(notdir ${SRC_Time}))
OBJ_Utils = $(patsubst %.cpp,%.o, $(notdir ${SRC_Utils}))
OBJ_Platforms = $(patsubst %.cpp,%.o, $(notdir ${SRC_Platforms}))
OBJ_Math = $(patsubst %.cpp,%.o, $(notdir ${SRC_Math}))
OBJ_Serialize = $(patsubst %.cpp,%.o, $(notdir ${SRC_Serialize}))

#  Core ECS Graphics Input ParticleSystem Physics Systems TextRender Time Utils
# Targets start here. 
Plutus.o: $(OBJ_Assets) $(OBJ_Core) $(OBJ_ECS) $(OBJ_Graphics) $(OBJ_Input) \
	$(OBJ_ParticleSystem) $(OBJ_Systems) $(OBJ_Time) $(OBJ_Utils) \
	$(OBJ_Platforms) $(OBJ_Math) $(OBJ_Physics) $(OBJ_Serialize)
	emcc $(shell find ./ -name "*.o") -r -o../../webbuild/Plutus.o

$(OBJ_Assets): ${SRC_Assets}
	${CC} -c ${SRC_Assets} -I../libs/stb -I../src -I../libs/freetype/include
$(OBJ_Core): ${SRC_Core}
	${CC} -c ${SRC_Core} ${CFLAGS}
$(OBJ_ECS): ${SRC_ECS}
	${CC} -c ${SRC_ECS} ${CFLAGS}
$(OBJ_Graphics): ${SRC_Graphics}
	${CC} -c ${SRC_Graphics} ${CFLAGS} -I../libs/glm
$(OBJ_Input): ${SRC_Input}
	${CC} ${SRC_Input} -I../src -c 
$(OBJ_ParticleSystem): ${SRC_ParticleSystem}
	${CC} -c ${SRC_ParticleSystem} -I../src
$(OBJ_Physics): ${SRC_Physics}
	${CC} -c ${SRC_Physics}  -I../src -I../libs/box2d/include -std=c++1z
$(OBJ_Systems): ${SRC_Systems}
	${CC} -c ${SRC_Systems} ${CFLAGS} -I../libs/glm
$(OBJ_Time): ${SRC_Time}
	${CC} -c ${SRC_Time} -I../src
$(OBJ_Utils): ${SRC_Utils}
	${CC} -c ${SRC_Utils} -I../src -I../libs/rapidjson/include -std=c++1z
$(OBJ_Platforms): ${SRC_Platforms}
	${CC} -c ${SRC_Platforms} -I../src -std=c++1z
$(OBJ_Math): ${SRC_Math}
	${CC} -c ${SRC_Math} -I../src
$(OBJ_Serialize): ${SRC_Serialize}
	${CC} -c ${SRC_Serialize}  ${CFLAGS} -I../src

clean: 
	rm -rf *.o