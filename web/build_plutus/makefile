

PLUTUS = ../../Plutus/src
LIBS =   ../../Plutus/libs

CFLAGS = -I${LIBS}/Entt \
	-I${LIBS}/lua/src \
	-I${LIBS}/rapidjson/include \
	-I${LIBS}/sol3/include \
	-I${LIBS}/stb \
	-I${PLUTUS} \
	-I${LIBS}/freetype/include \
	-I${LIBS}/box2d/include \
	-std=c++1z


CC = em++
# ----------------------Recursive Function ----------------------------------------
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
# --------------------------Get all cpp files ---------------------------------
SRC_Assets =         $(call rwildcard, ${PLUTUS}/Assets/, *.cpp)
SRC_Core =           $(call rwildcard, ${PLUTUS}/Core/, *.cpp)
SRC_ECS =            $(call rwildcard, ${PLUTUS}/ECS/, *.cpp)
SRC_Graphics =       $(call rwildcard, ${PLUTUS}/Graphics/, *.cpp)
SRC_Input =          $(call rwildcard, ${PLUTUS}/Input/, *.cpp)
SRC_ParticleSystem = $(call rwildcard, ${PLUTUS}/ParticleSystem/, *.cpp)
SRC_Physics =        $(call rwildcard, ${PLUTUS}/Physics/, *.cpp)
SRC_Systems =        $(call rwildcard, ${PLUTUS}/Systems/, *.cpp)
SRC_Time =           $(call rwildcard, ${PLUTUS}/Time/, *.cpp)
SRC_Utils =          $(call rwildcard, ${PLUTUS}/Utils/, *.cpp)
SRC_Platforms =      $(call rwildcard, ${PLUTUS}/Platforms/Web/, *.cpp)
SRC_Math =           $(call rwildcard, ${PLUTUS}/Math/, *.cpp)
SRC_Serialize =      $(call rwildcard, ${PLUTUS}/Serialize/, *.cpp)
# --------------------------convert path/filename.cpp to name.o ---------------------------------
OBJ_Assets =         $(patsubst %.cpp,%.o, $(notdir ${SRC_Assets}))
OBJ_Core =           $(patsubst %.cpp,%.o, $(notdir ${SRC_Core}))
OBJ_ECS =            $(patsubst %.cpp,%.o, $(notdir ${SRC_ECS}))
OBJ_Graphics =       $(patsubst %.cpp,%.o, $(notdir ${SRC_Graphics}))
OBJ_Input =          $(patsubst %.cpp,%.o, $(notdir ${SRC_Input}))
OBJ_ParticleSystem = $(patsubst %.cpp,%.o, $(notdir ${SRC_ParticleSystem}))
OBJ_Physics =        $(patsubst %.cpp,%.o, $(notdir ${SRC_Physics}))
OBJ_Systems =        $(patsubst %.cpp,%.o, $(notdir ${SRC_Systems}))
OBJ_Time =           $(patsubst %.cpp,%.o, $(notdir ${SRC_Time}))
OBJ_Utils =          $(patsubst %.cpp,%.o, $(notdir ${SRC_Utils}))
OBJ_Platforms =      $(patsubst %.cpp,%.o, $(notdir ${SRC_Platforms}))
OBJ_Math =           $(patsubst %.cpp,%.o, $(notdir ${SRC_Math}))
OBJ_Serialize =      $(patsubst %.cpp,%.o, $(notdir ${SRC_Serialize}))


# Targets start here. 
Plutus.o: $(OBJ_Assets) $(OBJ_Core) $(OBJ_ECS) $(OBJ_Graphics) $(OBJ_Input) \
	$(OBJ_ParticleSystem) $(OBJ_Systems) $(OBJ_Time) $(OBJ_Utils) \
	$(OBJ_Platforms) $(OBJ_Math) $(OBJ_Physics) $(OBJ_Serialize)
	emcc $(wildcard ./*.o) -r -o../Plutus.o

$(OBJ_Assets): ${SRC_Assets}
	${CC} -c ${SRC_Assets} -I${LIBS}/stb -I${PLUTUS} -I${LIBS}/freetype/include
$(OBJ_Core): ${SRC_Core}
	${CC} -c ${SRC_Core} ${CFLAGS}
$(OBJ_ECS): ${SRC_ECS}
	${CC} -c ${SRC_ECS} ${CFLAGS}
$(OBJ_Graphics): ${SRC_Graphics}
	${CC} -c ${SRC_Graphics} ${CFLAGS} -I${LIBS}/glm
$(OBJ_Input): ${SRC_Input}
	${CC} ${SRC_Input} -I${PLUTUS} -c 
$(OBJ_ParticleSystem): ${SRC_ParticleSystem}
	${CC} -c ${SRC_ParticleSystem} -I${PLUTUS}
$(OBJ_Physics): ${SRC_Physics}
	${CC} -c ${SRC_Physics}  -I${PLUTUS} -I${LIBS}/box2d/include -std=c++1z
$(OBJ_Systems): ${SRC_Systems}
	${CC} -c ${SRC_Systems} ${CFLAGS} -I${LIBS}/glm
$(OBJ_Time): ${SRC_Time}
	${CC} -c ${SRC_Time} -I${PLUTUS}
$(OBJ_Utils): ${SRC_Utils}
	${CC} -c ${SRC_Utils} -std=c++1z
$(OBJ_Platforms): ${SRC_Platforms}
	${CC} -c ${SRC_Platforms} -I${PLUTUS} -std=c++1z
$(OBJ_Math): ${SRC_Math}
	${CC} -c ${SRC_Math} -I${PLUTUS}
$(OBJ_Serialize): ${SRC_Serialize}
	${CC} -c ${SRC_Serialize}  ${CFLAGS} -I${PLUTUS}

ifeq ($(OS),Windows_NT)
clean: 
	IF EXIST "*.o" DEL "*.o" /s
else
clean: 
	rm -rf *.o
endif