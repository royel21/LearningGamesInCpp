const path = require("path");
const fs = require("fs");

const { buildOBJ, getObj, listFiles, buildOjbFiles } = require("./scripts/webbuilder");
const buildLib = require("./scripts/buildLibs");
buildLib();
// const plutusConfig = {
//   dirs: ["Plutus/src"],
//   excludedir: ["Windows", "Android"],
//   types: [".cpp"],
//   compiler: "emcc",
//   outdir: "web/Plutus",
//   include: [
//     "Plutus/src",
//     "Plutus/libs/glad/include",
//     "Plutus/libs/Entt",
//     "Plutus/libs/freetype/include",
//     "Plutus/libs/glm",
//     "Plutus/libs/lua/src",
//     "Plutus/libs/rapidjson/include",
//     "Plutus/libs/sol3/include",
//     "Plutus/libs/box2d/include",
//     "Plutus/libs/stb",
//   ],
//   objflags: "-std=c++17",
//   cflags: "-r",
//   exec: "Plutus.o",
// };

// libsConfig = {
//   dirs: ["Plutus/libs"],
//   excludedir: ["Windows", "Android"],
//   types: [".cpp"],
//   compiler: "emcc",
//   outdir: "web/Plutus",
//   include: [
//     "Plutus/src",
//     "Plutus/libs/glad/include",
//     "Plutus/libs/Entt",
//     "Plutus/libs/freetype/include",
//     "Plutus/libs/glm",
//     "Plutus/libs/lua/src",
//     "Plutus/libs/rapidjson/include",
//     "Plutus/libs/sol3/include",
//     "Plutus/libs/box2d/include",
//     "Plutus/libs/stb",
//   ],
//   objflags: "-std=c++17",
//   cflags: "-r",
//   exec: "Plutus.o",
// };
