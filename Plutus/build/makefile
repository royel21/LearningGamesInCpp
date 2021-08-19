box2d != find ../libs/box2d -name *.cpp
lua != find ../libs/lua -name *.c
stb != find ../libs/stb -name *.c

FT_DIR = ../libs/freetype/src
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

CFLAGS = -I../libs/lua/src -I../libs/box2d/include -I../libs/rapidjson/include -I../libs/sol3/include -I../libs/stb -I../libs/freetype/include --memory-init-file 0 -O3


buildlib: stb.o box2d.o lua.o FREETYPE
	emcc $(shell find . -name "*.o") -r -o../../webbuild/libs.o

stb.o: ${stb}
	emcc -c ${stb} ${CFLAGS}

box2d.o: ${box2d}
	emcc -c ${box2d} ${CFLAGS}

lua.o: ${lua}
	emcc -c ${lua} ${CFLAGS}

FREETYPE: ${FREETYPE_SRC}
	emcc -c ${FREETYPE_SRC} -I../libs/freetype/include -DFT2_BUILD_LIBRARY --memory-init-file 0 -O3

clean:
	rm  ./*.o