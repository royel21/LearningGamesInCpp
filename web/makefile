# --------------------------Helper Funtions------------------------
ifeq ($(OS),Windows_NT)
removeFile = $(shell del $(subst /,\, $1) /s /q)
else
removeFile = $(shell rm -f $1)
endif

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
getSRC = $(filter %$(patsubst %.o,/%.$(strip $3), $(notdir $1)), $2)
# --------------------------End Helper Funtions------------------------

all: Plutus Libs

PLUTUS = ../Plutus/src
LIBS =   ../Plutus/libs

PLUTUS_OBJ_DIR = obj/plutus

CFLAGS =  -I${LIBS}/Entt \
	-I${LIBS}/lua/src \
	-I${LIBS}/rapidjson/include \
	-I${LIBS}/sol3/include \
	-I${LIBS}/stb \
	-I${PLUTUS} \
	-I${LIBS}/freetype/include \
	-I${LIBS}/box2d/include \
	-I${LIBS}/freetype/include \
	-I${LIBS}/glad/include \
	-I${LIBS}/glm \
	-std=c++1z

SRC_Assets =  $(call rwildcard, ${PLUTUS}/Assets, *.cpp) 
SRC_Assets += $(call rwildcard, ${PLUTUS}/Core, *.cpp)
SRC_Assets += $(call rwildcard, ${PLUTUS}/ECS, *.cpp)
SRC_Assets += $(call rwildcard, ${PLUTUS}/Graphics, *.cpp)
SRC_Assets += $(call rwildcard, ${PLUTUS}/Input, *.cpp)
SRC_Assets += $(call rwildcard, ${PLUTUS}/ParticleSystem, *.cpp)
SRC_Assets += $(call rwildcard, ${PLUTUS}/Physics, *.cpp)
SRC_Assets += $(call rwildcard, ${PLUTUS}/Systems, *.cpp)
SRC_Assets += $(call rwildcard, ${PLUTUS}/Time, *.cpp)
SRC_Assets += $(call rwildcard, ${PLUTUS}/Utils, *.cpp)
SRC_Assets += $(call rwildcard, ${PLUTUS}/Platforms/Web, *.cpp)
SRC_Assets += $(call rwildcard, ${PLUTUS}/Math, *.cpp)
SRC_Assets += $(call rwildcard, ${PLUTUS}/Serialize, *.cpp)

PLUTUS_OBJS =  $(patsubst %.cpp,${PLUTUS_OBJ_DIR}/%.o, $(notdir ${SRC_Assets}))

Plutus: $(PLUTUS_OBJS)
	emcc $(wildcard ${PLUTUS_OBJ_DIR}/*.o) -r -o obj/Plutus.o


$(PLUTUS_OBJS): ${SRC_Assets}
	emcc -c $(call getSRC, $@, ${SRC_Assets}, cpp) -o $@ ${CFLAGS}

# *********************************** Libs ************************************* #
LIBS_OBJ_DIR = obj/libs

FT_DIR = ${LIBS}/freetype/src
# freetype
FREETYPE_SRC = ${FT_DIR}/autofit/autofit.c \
	${FT_DIR}/base/ftbase.c \
	${FT_DIR}/base/ftbbox.c \
	${FT_DIR}/base/ftbdf.c \
	${FT_DIR}/base/ftbitmap.c \
	${FT_DIR}/base/ftcid.c \
	${FT_DIR}/base/ftfstype.c \
	${FT_DIR}/base/ftgasp.c \
	${FT_DIR}/base/ftglyph.c \
	${FT_DIR}/base/ftgxval.c \
	${FT_DIR}/base/ftinit.c \
	${FT_DIR}/base/ftmm.c \
	${FT_DIR}/base/ftotval.c \
	${FT_DIR}/base/ftpatent.c \
	${FT_DIR}/base/ftpfr.c \
	${FT_DIR}/base/ftstroke.c \
	${FT_DIR}/base/ftsynth.c \
	${FT_DIR}/base/fttype1.c \
	${FT_DIR}/base/ftwinfnt.c \
	${FT_DIR}/bdf/bdf.c \
	${FT_DIR}/bzip2/ftbzip2.c \
	${FT_DIR}/cache/ftcache.c \
	${FT_DIR}/cff/cff.c \
	${FT_DIR}/cid/type1cid.c \
	${FT_DIR}/gzip/ftgzip.c \
	${FT_DIR}/lzw/ftlzw.c \
	${FT_DIR}/pcf/pcf.c \
	${FT_DIR}/pfr/pfr.c \
	${FT_DIR}/psaux/psaux.c \
	${FT_DIR}/pshinter/pshinter.c \
	${FT_DIR}/psnames/psnames.c \
	${FT_DIR}/raster/raster.c \
	${FT_DIR}/sdf/sdf.c \
	${FT_DIR}/sfnt/sfnt.c \
	${FT_DIR}/smooth/smooth.c \
	${FT_DIR}/truetype/truetype.c \
	${FT_DIR}/type1/type1.c \
	${FT_DIR}/type42/type42.c \
	${FT_DIR}/winfonts/winfnt.c \
	${FT_DIR}/base/ftsystem.c \
	${FT_DIR}/base/ftdebug.c 
	
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

box2d = $(call rwildcard, ${LIBS}/box2d/src/, *.cpp)
lua   = $(call rwildcard, ${LIBS}/lua/src/, *.c)
stb   = $(call rwildcard, ${LIBS}/stb/, *.c)

OBJ_BOX2D = $(patsubst %.cpp,${LIBS_OBJ_DIR}/%.o, $(notdir ${box2d}))
OBJ_LUA = $(patsubst %.c,${LIBS_OBJ_DIR}/%.o, $(notdir ${lua}))
OBJ_STB = $(patsubst %.c,${LIBS_OBJ_DIR}/%.o, $(notdir ${stb}))
OBJ_FREETYPE = $(patsubst %.c,${LIBS_OBJ_DIR}/%.o, $(notdir ${FREETYPE_SRC}))


Libs: $(OBJ_BOX2D) $(OBJ_LUA) $(OBJ_STB) $(OBJ_FREETYPE)
	emcc $(wildcard ${LIBS_OBJ_DIR}/*.o) -r -o obj/Libs.o

$(OBJ_BOX2D): ${box2d}
	emcc -c $(call getSRC, $@, ${box2d}, cpp) -o $@ ${CFLAGS} -I${LIBS}/box2d/include

$(OBJ_LUA): ${lua}
	 emcc -c $(call getSRC, $@, ${lua}, c) -o $@  -I${LIBS}/lua/src

$(OBJ_STB): ${stb}
	emcc -c $(call getSRC, $@, ${stb}, c) -o $@  -I${LIBS}/stb

$(OBJ_FREETYPE): ${FREETYPE_SRC}
	emcc -c $(call getSRC,$@,${FREETYPE_SRC}, c) -o $@ -I${LIBS}/freetype/include -DFT2_BUILD_LIBRARY


clean-lib:
	$(call removeFile, ./obj/libs/*.o)
clean-plutus:
	$(call removeFile, ./obj/plutus/*.o)
