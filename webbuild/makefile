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

CFLAGS = -s USE_WEBGL2=1 -s USE_GLFW=3 -s WASM=1 -std=c++1z

SRC != find ../src -name "*.cpp"

GAME_OBJ != find . -name "*.o"

output: game.o
	emcc ${GAME_OBJ} -o release/index.html ${CFLAGS} ${INCLUDE} --preload-file ./assets

game.o: ${SRC}
	emcc -c ${SRC} ${INCLUDE}

# include ../Plutus/pbuild/makefile