const { execSync } = require("child_process");
const path = require("path");
const fs = require("fs-extra");

const { buildProject, cleanProject } = require("./scripts/build-project");
const { buildLib, cleanLibs } = require("./scripts/build-plutus-libs");

const liboutdir = path.join("web", "obj");

const buildAll = () => {
  //----------------------------------------------Build Plutus Libs--------------------------------------------
  const libs = buildLib({ outdir: liboutdir });

  //----------------------------------------------Build Plutus--------------------------------------------
  const include = [
    "Plutus/src",
    "Plutus/libs/glad/include",
    "Plutus/libs/Entt",
    "Plutus/libs/freetype/include",
    "Plutus/libs/glm",
    "Plutus/libs/lua/src",
    "Plutus/libs/rapidjson/include",
    "Plutus/libs/sol3/include",
    "Plutus/libs/box2d/include",
    "Plutus/libs/stb",
  ];

  const config = {
    name: "plutus",
    compiler: "emcc",
    outdir: liboutdir,
    cflags: "-std=c++17",
    include,
    src: [path.join("Plutus", "src")],
    filetype: [".cpp"],
    sync: true,
    excludeDir: ["Windows", "Android"],
  };

  const plutus = buildProject(config);
  //----------------------------------------------Project Game--------------------------------------------
  const projectName = "AssetTest";
  config.name = projectName;
  config.src = [path.join("Projects", projectName)];

  const game = buildProject(config);
  //----------------------------------------------End Project Game--------------------------------------------

  if (!game.isUptodate || !plutus.isUptodate) {
    const outExecDir = path.join("web", "release");
    if (!fs.existsSync(outExecDir)) {
      fs.mkdirs(outExecDir);
    }

    const OUTPUT_FLAG = `--shell-file web/game.html --preload-file ./assets -s WASM=1 -s ALLOW_MEMORY_GROWTH=1`;

    const CFLAGS = `-s USE_WEBGL2=1 -s USE_GLFW=3 -lopenal -s WASM=1 -std=c++1z`;

    const EXEC = path.join(outExecDir, "index.html");

    try {
      const INCLUDE = include.map((i) => `-I${i}`).join(" ");

      const cmd = `emcc ${game.lib} ${plutus.lib} ${libs} -o ${EXEC} ${CFLAGS} ${OUTPUT_FLAG} ${INCLUDE}`;
      console.log(cmd);
      execSync(cmd);
    } catch (error) {}
  } else {
    console.log("Project is up to date");
  }
};

const cmd = process.argv[2];

console.log(cmd);

const objDir = path.join("web", "obj");

const options = {
  cleanLibs,
  "clean-all": () => cleanProject(objDir),
  "clean-libs": () => cleanProject(objDir, "libs"),
  "clean-plutus": () => cleanProject(objDir, "plutus"),
  "clean-game": () => cleanProject(objDir, projectName),
};

if (!cmd) {
  buildAll();
} else {
  let action = options[cmd];
  if (action) action();
}
