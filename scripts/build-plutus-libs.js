const path = require("path");
const fs = require("fs");

const { execSync } = require("child_process");
const { listFiles, buildOjbFiles } = require("./helper");

const buildLib = ({ outdir = "build/obj", rebuild = false }) => {
  const outLib = path.join(outdir, "libs.o");

  if (!rebuild && fs.existsSync(outLib)) {
    return outLib;
  }

  const obj_outdir = path.join(outdir, "libs");

  const FT_DIR = path.join("Plutus", "libs", "freetype");
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
  ].map((ft) => path.join(FT_DIR, "src", ft.a, ft.b));

  const FT_Config = {
    compiler: "emcc",
    cflags: "-DFT2_BUILD_LIBRARY",
    outdir: obj_outdir,
    files: FT_SRC,
    include: [path.join(FT_DIR, "include")],
    rebuild,
  };

  const PlutusDir = path.join("Plutus", "libs");

  const Libs = {
    compiler: "emcc",
    cflags: "-std=c++1z",
    outdir: obj_outdir,
    rebuild,
  };
  // ------Build Freetype
  buildOjbFiles(FT_Config);

  // ------Build Box2d
  let files = listFiles([path.join(PlutusDir, "box2d", "src")], ".cpp");
  let include = [path.join(PlutusDir, "box2d", "include")];
  buildOjbFiles({ ...Libs, include, files });

  //-------Build Lua, STB
  const luaSrc = listFiles(path.join(PlutusDir, "lua", "src"), ".c");
  files = [...listFiles(path.join(PlutusDir, "stb"), ".c"), ...luaSrc];
  include = [path.join(PlutusDir, "lua", "src"), path.join(PlutusDir, "stb")];
  buildOjbFiles({ ...Libs, files, include, cflags: "" });

  const libObjs = listFiles(obj_outdir, ".o");

  try {
    let cmd = `emcc -r ${libObjs.join(" ")} -o ${outLib}`;
    console.log(cmd);
    execSync(cmd);
  } catch (err) {
    return;
  }
  return outLib;
};

const cleanLibs = ({ outdir }) => {
  fs.rmdirSync(path.join(outdir, "obj", "libs"), { recursive: true, force: true });
  fs.rmSync(path.join(outdir, "obj", "libs.o"));
};

module.exports = { buildLib, cleanLibs };
