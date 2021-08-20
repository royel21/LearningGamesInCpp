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
	
OUTPUT_FLAG =  --shell-file game.html --preload-file ./assets -s WASM=1 

SRC_Plutus != find ../Plutus/src -name "*.cpp" -not -path ../Plutus/src/Platform/Windows

CFLAGS = -s USE_WEBGL2=1 -s USE_GLFW=3 -s WASM=1 -std=c++1z

SRC != find ../src -name "*.cpp"

GAME_OBJ = $(patsubst %.cpp,%.o, $(notdir ${SRC}))

output: Plutus.o libs.o $(GAME_OBJ) 
	emcc ${GAME_OBJ} Plutus.o libs.o -o release/index.html ${CFLAGS} ${INCLUDE} ${OUTPUT_FLAG}

$(GAME_OBJ): ${SRC}
	emcc -c ${SRC} ${INCLUDE}

Plutus.o: ${SRC_Plutus}
	cd ../Plutus/pbuild&&emmake make

libs.o:
	cd ../Plutus/build&&emmake make

$(shell mkdir -p release)