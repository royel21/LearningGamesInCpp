const fs = require("fs-extra");
const path = require("path");
const { execSync } = require("child_process");

const { listFiles, buildOjbFiles, getChanges, checkHeaders, db } = require("./helper");

const conf = {
  name: "",
  compiler: "",
  outdir: "",
  cflags: "",
  include: [],
  src: [],
  filetype: [],
  sync: true,
  excludeDir: [],
};

const buildProject = (config = conf) => {
  let isUptodate = true;

  const { compiler, name, outdir, excludeDir, src, filetype } = config;

  const obj_outdir = path.join(outdir, name);

  const sources = listFiles(src, filetype, excludeDir);
  const headers = listFiles(src, ".h", excludeDir);

  //find changes in .cpp or .h and return only what cpp was changed
  let files = getChanges(sources, obj_outdir);
  files = checkHeaders(files, headers, sources, obj_outdir);

  //build obj files from src files
  buildOjbFiles({ ...config, outdir: obj_outdir, files });

  const outLib = path.join(outdir, name + ".o");

  const objects = listFiles(obj_outdir, ".o");
  if (files.length || (objects.length && !fs.existsSync(outLib))) {
    try {
      let cmd = `${compiler} -r ${objects.join(" ")} -o ${outLib}`;
      console.log(outLib);
      execSync(cmd);
    } catch (err) {
      console.log(err);
      return;
    }
    isUptodate = false;
  }

  return { lib: outLib, isUptodate };
};

const cleanProject = (dir, name) => {
  console.log("removing - " + name || "all", "obj files");
  const toDel = name ? path.join(dir, name) : dir;

  const files = listFiles(toDel, ".o");
  for (const f of files) {
    fs.removeSync(f);
    console.log(`rm ${f}`);
  }

  if (name && fs.existsSync(toDel + ".o")) {
    fs.removeSync(toDel + ".o");
    console.log(`rm ${toDel}.o`);
  }
};

module.exports = { buildProject, cleanProject };
