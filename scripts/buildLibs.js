const path = require("path");
const fs = require("fs");

const { execSync } = require("child_process");
const { listFiles, buildOjbFiles } = require("./webbuilder");

module.exports = () => {
  const FT_DIR = path.join("Plutus", "libs", "freetype", "src");
  const FT_SRC = [
    { a: "autofit", b: "autofit.c" },
    { a: "base", b: "ftbase.c" },
    { a: "base", b: "ftbbox.c" },
    { a: "base", b: "ftbdf.c" },
    { a: "base", b: "ftbitmap.c" },
    { a: "base", b: "ftcid.c" },
    { a: "base", b: "ftfstype.c" },
    { a: "base", b: "ftgasp.c" },
    { a: "base", b: "ftglyph.c" },
    { a: "base", b: "ftgxval.c" },
    { a: "base", b: "ftinit.c" },
    { a: "base", b: "ftmm.c" },
    { a: "base", b: "ftotval.c" },
    { a: "base", b: "ftpatent.c" },
    { a: "base", b: "ftpfr.c" },
    { a: "base", b: "ftstroke.c" },
    { a: "base", b: "ftsynth.c" },
    { a: "base", b: "fttype1.c" },
    { a: "base", b: "ftwinfnt.c" },
    { a: "bdf", b: "bdf.c" },
    { a: "bzip2", b: "ftbzip2.c" },
    { a: "cache", b: "ftcache.c" },
    { a: "cff", b: "cff.c" },
    { a: "cid", b: "type1cid.c" },
    { a: "gzip", b: "ftgzip.c" },
    { a: "lzw", b: "ftlzw.c" },
    { a: "pcf", b: "pcf.c" },
    { a: "pfr", b: "pfr.c" },
    { a: "psaux", b: "psaux.c" },
    { a: "pshinter", b: "pshinter.c" },
    { a: "psnames", b: "psnames.c" },
    { a: "raster", b: "raster.c" },
    { a: "sdf", b: "sdf.c" },
    { a: "sfnt", b: "sfnt.c" },
    { a: "smooth", b: "smooth.c" },
    { a: "truetype", b: "truetype.c" },
    { a: "type1", b: "type1.c" },
    { a: "type42", b: "type42.c" },
    { a: "winfonts", b: "winfnt.c" },
    { a: "base", b: "ftsystem.c" },
    { a: "base", b: "ftdebug.c" },
  ].map((ft) => path.join(FT_DIR, ft.a, ft.b));

  const FT_Config = {
    compiler: "emcc",
    cflags: "-DFT2_BUILD_LIBRARY",
    outdir: "web/obj/libs",
    files: FT_SRC,
    include: [path.join(FT_DIR, "include")],
  };

  const PlutusDir = path.join("Plutus", "libs");

  const boxsrc = listFiles([path.join(PlutusDir, "box2d", "src")], ".cpp");
  const luasrc = listFiles(path.join(PlutusDir, "lua", "src"), ".c");
  const stbSrc = listFiles(path.join(PlutusDir, "stb"), ".c");

  const Libs = {
    compiler: "emcc",
    cflags: "-std=c++1z",
    outdir: "web/obj/libs",
    files: [...boxsrc, ...luasrc, ...stbSrc],
    include: [
      path.join(PlutusDir, "box2d", "include"),
      path.join(PlutusDir, "lua", "src"),
      path.join(PlutusDir, "stb"),
    ],
  };

  let objs = buildOjbFiles(FT_Config);
  let obj2 = buildOjbFiles(Libs);

  try {
    let cmd = `emcc -c ${[...objs, ...obj2].map((o) => path.join(objPath, o)).join(" ")} -o web/libs/Libs.o`;
    console.log(cmd);
    execSync(cmd);
  } catch (err) {
    return;
  }
};
