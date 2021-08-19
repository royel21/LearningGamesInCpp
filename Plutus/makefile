
CFLAGS = -Ilibs/Entt \
	-Ilibs/glm \
	-Ilibs/lua/src \
	-Ilibs/rapidjson/include \
	-Ilibs/sol3/include \
	-Ilibs/stb \
	-Isrc \
	--memory-init-file 0 -O3 \
	-std=c++1z

SRC_Assets != find src/Assets -name *.cpp
SRC_Core != find src/Core -name *.cpp
SRC_ECS != find src/ECS -name *.cpp
SRC_Graphics != find src/Graphics -name *.cpp
SRC_Input != find src/Input -name *.cpp
SRC_ParticleSystem != find src/ParticleSystem -name *.cpp
SRC_Physics != find src/Physics -name *.cpp
SRC_Systems != find src/Systems -name *.cpp
SRC_TextRender != find src/TextRender -name *.cpp
SRC_Time != find src/Time -name *.cpp
SRC_Utils != find src/Utils -name *.cpp

OBJ_Assets = $(patsubst %.cpp,%.o, $(addprefix build/,$(notdir ${SRC_Assets})))
OBJ_Core = $(patsubst %.cpp,%.o, $(addprefix build/,$(notdir ${SRC_Core})))
OBJ_ECS = $(patsubst %.cpp,%.o, $(addprefix build/,$(notdir ${SRC_ECS})))
OBJ_Graphics = $(patsubst %.cpp,%.o, $(addprefix build/,$(notdir ${SRC_Graphics})))
OBJ_Input = $(patsubst %.cpp,%.o, $(addprefix build/,$(notdir ${SRC_Input})))
OBJ_ParticleSystem = $(patsubst %.cpp,%.o, $(addprefix build/,$(notdir ${SRC_ParticleSystem})))
OBJ_Physics = $(patsubst %.cpp,%.o, $(addprefix build/,$(notdir ${SRC_Physics})))
OBJ_Systems = $(patsubst %.cpp,%.o, $(addprefix build/,$(notdir ${SRC_Systems})))
OBJ_TextRender = $(patsubst %.cpp,%.o, $(addprefix build/,$(notdir ${SRC_TextRender})))
OBJ_Time = $(patsubst %.cpp,%.o, $(addprefix build/,$(notdir ${SRC_Time})))
OBJ_Utils = $(patsubst %.cpp,%.o, $(addprefix build/,$(notdir ${SRC_Utils})))

Plutus: $(OBJ_Assets) \
	$(OBJ_Core) \
	$(OBJ_ECS) \
	$(OBJ_Graphics) \
	$(OBJ_Input) \
	$(OBJ_ParticleSystem) \
	$(OBJ_Physics) \
	$(OBJ_Systems) \
	$(OBJ_TextRender) \
	$(OBJ_Time) \
	$(OBJ_Utils) \

	emcc $(shell find build/ -name "*.o") -r -o ../webbuild

$(OBJ_Assets): %.o: $(filter %.cpp, ${SRC_Assets})
	emcc -c ${CFLAGS} $< -o $@

$(OBJ_Core): %.o: $(filter %.cpp, ${SRC_Core})
	emcc -c ${CFLAGS} $< -o $@

$(OBJ_ECS): %.o: $(filter %.cpp, ${SRC_ECS})
	emcc -c ${CFLAGS} $< -o $@

$(OBJ_Graphics): %.o: $(filter %.cpp, ${SRC_Graphics})
	emcc -c ${CFLAGS} $< -o $@

$(OBJ_Input): %.o: $(filter %.cpp, ${SRC_Input})
	emcc -c ${CFLAGS} $< -o $@

$(OBJ_ParticleSystem): %.o: $(filter %.cpp, ${SRC_ParticleSystem})
	emcc -c ${CFLAGS} $< -o $@

$(OBJ_Physics): %.o: $(filter %.cpp, ${SRC_Physics})
	emcc -c ${CFLAGS} $< -o $@

$(OBJ_Systems): %.o: $(filter %.cpp, ${SRC_Systems})
	emcc -c ${CFLAGS} $< -o $@

$(OBJ_TextRender): %.o: $(filter %.cpp, ${SRC_TextRender})
	emcc -c ${CFLAGS} $< -o $@ -Ilibs/freetype/include

$(OBJ_Time): %.o: $(filter %.cpp, ${SRC_Time})
	emcc -c ${CFLAGS} $< -o $@

$(OBJ_Utils): %.o: $(filter %.cpp, ${SRC_Utils})
	emcc -c ${CFLAGS} $< -o $@
clean:
