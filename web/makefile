# Create output dir

LIBS = Plutus/libs

INCLUDE = -I../Plutus/libs/box2d/include \
	-I../Plutus/libs/Entt \
	-I../Plutus/libs/freetype/include \
	-I../Plutus/libs/glm \
	-I../Plutus/libs/lua/src \
	-I../Plutus/libs/rapidjson/include \
	-I../Plutus/libs/sol3/include \
	-I../Plutus/libs/stb \
	-I../Plutus/src \
	-I./src \
	--memory-init-file 0 -O3 \
	-std=c++1z
	
OUTPUT_FLAG =  --shell-file game.html --preload-file ./assets -s WASM=1 -s ALLOW_MEMORY_GROWTH=1

CFLAGS = -s USE_WEBGL2=1 -s USE_GLFW=3 -lopenal -s WASM=1 -std=c++1z

SRC = $(wildcard ../game/*.cpp) $(wildcard ../game/Screens/*.cpp) 

GAME_OBJ = $(patsubst %.cpp,%.o, $(notdir ${SRC}))

output: Plutus libs.o $(GAME_OBJ) MakeDir
	emcc ${GAME_OBJ} Plutus.o libs.o -o release/index.html ${CFLAGS} ${INCLUDE} ${OUTPUT_FLAG}

$(GAME_OBJ): ${SRC}
	emcc -c ${SRC} ${INCLUDE}

Plutus:
	cd ../Plutus/pbuild&&emmake make

libs.o:
	cd ../Plutus/build&&emmake make

MakeDir:
ifeq ($(OS),Windows_NT)
  $(shell IF NOT EXIST realse mkdir release)
else
  $(shell mkdir -p release)
endif

ifeq ($(OS),Windows_NT)
clean: 
	IF EXIST "*.o" DEL "*.o" /s
else
clean: 
	rm -rf *.o
endif
