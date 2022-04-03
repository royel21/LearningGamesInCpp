# # Create output dir
ifeq ($(OS),Windows_NT)
makedir = $(shell IF NOT EXIST $(subst /,\, $1) mkdir $(subst /,\, $1))
removeFile = $(shell del $(subst /,\, $1) /s /q)
else
makedir = $(shell mkdir -p $1)
removeFile = $(shell rm -f $1)
endif

$(call makedir, web/release)
$(call makedir, web/obj/libs)
$(call makedir, web/obj/plutus)
$(call makedir, web/obj/game)


# # ----------------------Recursive Function ----------------------------------------
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

getSRC = $(filter %$(patsubst %.o,/%.cpp, $(notdir $1)), $2)

LIBS = Plutus/libs
PLUTUS_SRC = Plutus/src
gamedir = Projects/game

INCLUDE = -I${LIBS}/box2d/include \
	-I${LIBS}/Entt \
	-I${LIBS}/freetype/include \
	-I${LIBS}/glm \
	-I${LIBS}/lua/src \
	-I${LIBS}/rapidjson/include \
	-I${LIBS}/sol3/include \
	-I${LIBS}/stb \
	-I${PLUTUS_SRC} \
	-I./src \
	--memory-init-file 0 -O3 \
	-std=c++1z
	
OUTPUT_FLAG =  --shell-file web/game.html --preload-file ./assets -s WASM=1 -s ALLOW_MEMORY_GROWTH=1

CFLAGS = -s USE_WEBGL2=1 -s USE_GLFW=3 -lopenal -s WASM=1 -std=c++1z

SRC = $(call rwildcard, ${gamedir}, *.cpp)  
GAME_OBJ = $(patsubst %.cpp, web/obj/game/%.o, $(notdir ${SRC}))

ENGINE_OBJ = web/obj/Plutus.o web/obj/libs.o

output:  $(ENGINE_OBJ) $(GAME_OBJ)
	emcc ${ENGINE_OBJ} ${GAME_OBJ} -o web/release/index.html ${CFLAGS} ${INCLUDE} ${OUTPUT_FLAG}

$(GAME_OBJ): ${SRC}
	emcc -c $(call getSRC, $@, ${SRC}) -o $@ ${INCLUDE}

$(ENGINE_OBJ): 
	cd web&&emmake make

clean:
	$(call removeFile, web/*.o)
