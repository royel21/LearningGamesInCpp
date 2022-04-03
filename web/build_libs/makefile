
LIBS_P = ../../Plutus/libs

FT_DIR = ${LIBS_P}/freetype/src
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

box2d = $(call rwildcard, ${LIBS_P}/box2d/src/, *.cpp)
lua   = $(call rwildcard, ${LIBS_P}/lua/src/, *.cpp)
stb   = $(call rwildcard, ${LIBS_P}/stb/, *.cpp)

OBJ_BOX2D = $(patsubst %.cpp,%.o, $(notdir ${box2d}))
OBJ_LUA = $(patsubst %.c,%.o, $(notdir ${lua}))
OBJ_STB = $(patsubst %.c,%.o, $(notdir ${stb}))
OBJ_FREETYPE = $(patsubst %.c,%.o, $(notdir ${FREETYPE_SRC}))

CFLAGS = --memory-init-file 0 -O3

libs.o: $(OBJ_BOX2D) $(OBJ_LUA) $(OBJ_STB) $(OBJ_FREETYPE)
	emcc $(wildcard ./*.o) -r -o../libs.o

$(OBJ_BOX2D): ${box2d}
	emcc -c ${box2d} ${CFLAGS} -I${LIBS_P}/box2d/include

$(OBJ_LUA): ${lua}
	emcc -c ${lua} ${CFLAGS} -I${LIBS_P}/lua/src

$(OBJ_STB): ${stb}
	emcc -c ${stb} ${CFLAGS} -I${LIBS_P}/stb


$(OBJ_FREETYPE): ${FREETYPE_SRC}
	emcc -c ${FREETYPE_SRC} ${CFLAGS} -I${LIBS_P}/freetype/include -DFT2_BUILD_LIBRARY

ifeq ($(OS),Windows_NT)
clean: 
	IF EXIST "*.o" DEL "*.o" /s
else
clean: 
	rm -rf *.o
endif
