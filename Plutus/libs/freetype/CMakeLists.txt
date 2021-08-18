# CMakeLists.txt
#
# Copyright (C) 2013-2021 by
# David Turner, Robert Wilhelm, and Werner Lemberg.
#
# Written originally by John Cary <cary@txcorp.com>
#
# This file is part of the FreeType project, and may only be used, modified,
# and distributed under the terms of the FreeType project license,
# LICENSE.TXT.  By continuing to use, modify, or distribute this file you
# indicate that you have read the license and understand and accept it
# fully.
#
#
# The following will (1) create a build directory, and (2) change into it and
# call cmake to configure the build with default parameters as a static
# library.  See
#
#   https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html
#
# for information about debug or release builds, for example
#
#   cmake -B build -D CMAKE_BUILD_TYPE=Release
#
#
# For a dynamic library, use
#
#   cmake -B build -D BUILD_SHARED_LIBS=true -D CMAKE_BUILD_TYPE=Release
#
# For a framework on OS X, use
#
#   cmake -E chdir build cmake -G Xcode -D BUILD_FRAMEWORK:BOOL=true ..
#
# For an iOS static library, use
#
#   cmake -E chdir build cmake -G Xcode -D IOS_PLATFORM=OS ..
#
# or
#
#   cmake -E chdir build cmake -G Xcode -D IOS_PLATFORM=SIMULATOR ..
#
# or
#
#   cmake -E chdir build cmake -G Xcode -D IOS_PLATFORM=SIMULATOR64 ..
#
#
# Finally, build the project with
#
#   cmake --build build
#
# Install it with
#
#   (sudo) cmake --build build --target install
#
# A binary distribution can be made with
#
#   cmake --build build --config Release --target package
#
# Please refer to the cmake manual for further options, in particular, how
# to modify compilation and linking parameters.
#
# Some notes.
#
# - `cmake' creates configuration files in
#
#     <build-directory>/include/freetype/config
#
#   which should be further modified if necessary.
#
# - You can use `cmake' directly on a freshly cloned FreeType git
#   repository.
#
# - `CMakeLists.txt' is provided as-is since it is normally not used by the
#   developer team.
#
# - Set the `FT_REQUIRE_ZLIB', `FT_REQUIRE_BZIP2', `FT_REQUIRE_PNG',
#   `FT_REQUIRE_HARFBUZZ', and `FT_REQUIRE_BROTLI' CMake variables to `ON'
#   or `TRUE' to force using a dependency.  Leave a variable undefined
#   (which is the default) to use the dependency only if it is available.
#   Example:
#
cmake_minimum_required(VERSION 3.10)

file(GLOB PUBLIC_HEADERS "include/ft2build.h" "include/freetype/*.h")
file(GLOB PUBLIC_CONFIG_HEADERS "include/freetype/config/*.h")
file(GLOB PRIVATE_HEADERS "include/freetype/internal/*.h")

include_directories(include)

set(BASE_SRCS
  src/autofit/autofit.c
  src/base/ftbase.c
  src/base/ftbbox.c
  src/base/ftbdf.c
  src/base/ftbitmap.c
  src/base/ftcid.c
  src/base/ftfstype.c
  src/base/ftgasp.c
  src/base/ftglyph.c
  src/base/ftgxval.c
  src/base/ftinit.c
  src/base/ftmm.c
  src/base/ftotval.c
  src/base/ftpatent.c
  src/base/ftpfr.c
  src/base/ftstroke.c
  src/base/ftsynth.c
  src/base/fttype1.c
  src/base/ftwinfnt.c
  src/bdf/bdf.c
  src/bzip2/ftbzip2.c
  src/cache/ftcache.c
  src/cff/cff.c
  src/cid/type1cid.c
  src/gzip/ftgzip.c
  src/lzw/ftlzw.c
  src/pcf/pcf.c
  src/pfr/pfr.c
  src/psaux/psaux.c
  src/pshinter/pshinter.c
  src/psnames/psnames.c
  src/raster/raster.c
  src/sdf/sdf.c
  src/sfnt/sfnt.c
  src/smooth/smooth.c
  src/truetype/truetype.c
  src/type1/type1.c
  src/type42/type42.c
  src/winfonts/winfnt.c
)

if (UNIX)
  list(APPEND BASE_SRCS "builds/unix/ftsystem.c")
elseif (WIN32)
  list(APPEND BASE_SRCS "builds/windows/ftsystem.c")
else ()
  list(APPEND BASE_SRCS "src/base/ftsystem.c")
endif ()

if (WIN32)
  enable_language(RC)
  list(APPEND BASE_SRCS builds/windows/ftdebug.c
                        src/base/ftver.rc)
elseif (WINCE)
  list(APPEND BASE_SRCS builds/wince/ftdebug.c)
else ()
  list(APPEND BASE_SRCS src/base/ftdebug.c)
endif ()


add_library(freetype ${BASE_SRCS} ${PUBLIC_HEADERS} ${PRIVATE_HEADER} ${PUBLIC_CONFIG_HEADERS})

target_compile_definitions(freetype PRIVATE FT2_BUILD_LIBRARY)

if (WIN32)
  target_compile_definitions(
    freetype PRIVATE _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS)
endif ()