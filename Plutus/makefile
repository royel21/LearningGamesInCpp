CFLAGS = -Ilibs/Entt \
	-Ilibs/glm \
	-Ilibs/stb\
	--memory-init-file 0 -O3 \
	 -Ilibs/lua/src \
	-Ilibs/sol3/include \
	-Ilibs/freetype/include \
	-Ilibs/rapidjson/include \
	-Isrc \
	-std=c++1z

BUILD_DIR = build/PlutusObjs
SRC_DIR = src
SOURCES = $(shell find src/ -name "*.cpp")
TARGET  = ../webbuild/PLutus.o
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/$(notdir %).o)

default: ${TARGET}

.SECONDEXPANSION:

$(OBJECTS) : $$(patsubst $(BUILD_DIR)/$(notdir %).o,$(SRC_DIR)/%.cpp,$$@)
	$(CC) -c $< $(CFLAGS) -o ${BUILD_DIR}/$(notdir $@)

$(TARGET): $(OBJECTS)
# $(CC) -r -o $@ $^

.PHONY: default

clean:
	rm -rf ${DIR_P}/*.o
