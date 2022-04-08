const { execSync } = require("child_process");

const fs = require("fs-extra");
const path = require("path");

let db = { files: {}, headers: {} };

if (fs.existsSync("db.json")) {
  db = fs.readJSONSync("db.json");
}

const getArray = (d) => (d instanceof Array ? d : [d]);

/**
 * @param dirs can be a list of dir ['dir/1', 'dir/2'] or a single dir
 * @param types list of type [.cpp, .c, .cc] or sinlge type
 * @return a list of [path/to/a.cpp, path/to/b.c]
 */

const listFiles = (dirs, types, excludedir = []) => {
  const src = [];
  if (!types) return [];
  types = getArray(types);
  dirs = getArray(dirs);

  const checkExclude = (file) => {
    for (let ex of excludedir) {
      if (file.includes(ex)) return true;
    }
  };

  const listsrc = (dir) => {
    if (fs.existsSync(dir)) {
      let files = fs.readdirSync(dir);
      for (let f of files) {
        let file = path.join(dir, f);
        if (fs.statSync(file).isDirectory()) {
          if (!checkExclude(file)) listsrc(file, types);
        } else {
          for (let type of types)
            if (file.endsWith(type)) {
              src.push(file);
            }
        }
      }
    }
  };

  dirs.forEach((dir) => listsrc(dir));
  return src;
};

const getObj = (outdir, n) => {
  let name = path.basename(n);
  name = name.substring(0, name.lastIndexOf("."));
  return path.join(outdir, name + ".o");
};

const checkHeaders = (files, headers, sources, objDir) => {
  let srcs = sources.filter((f) => !files.find((f2) => f2.path === f));

  for (const h of headers) {
    let lastMod = fs.statSync(h).mtime.getTime() - 2000;
    if (db.headers[h] !== lastMod) {
      db.headers[h] = lastMod;
      let name = path.basename(h);
      for (let i = srcs.length - 1; i > 0; i--) {
        const src = srcs[i];

        const content = fs.readFileSync(src, { encoding: "utf-8" });
        if (content.includes(name)) {
          let obj = getObj(objDir, src);
          files.push({
            path: src,
            obj,
          });
          srcs = srcs.filter((s) => s != src);
        }
      }
    }

    if (srcs.length === 0) break;
  }
  return files;
};

const getChanges = (files, objDir) => {
  let changes = [];
  for (const file of files) {
    let lastmod = fs.statSync(file).mtime.getTime() - 2000;
    let obj = getObj(objDir, file);

    if (lastmod !== db.files[file] || !fs.existsSync(obj)) {
      db.files[file] = lastmod;
      changes.push({ path: file, obj });
    }
  }

  return changes;
};

const buildOjbFiles = ({ compiler, include, outdir, files, cflags }) => {
  let includes = include.map((i) => `-I${i}`).join(" ");

  if (!fs.existsSync(outdir)) {
    fs.mkdirs(outdir);
  }
  const objects = files.map((f) => ({ filePath: f, objPath: getObj(outdir, f) }));

  for (let src of objects) {
    try {
      let cmd = `${compiler} -c ${src.filePath} -o ${src.objPath} ${includes} ${cflags}`;
      console.log(cmd);
      execSync(cmd);
    } catch (err) {
      return [];
    }
  }
  return objects.map((o) => o.objPath);
};

const buildOBJ = (config) => {
  const { dirs, excludedir, types, compiler, outdir = "build", include = [], cflags, exec, objflags } = config;
  console.log(`------- building ${exec} -----------`);
  const sources = listFiles(dirs, types, excludedir);
  const headers = listFiles(dirs, ".h", excludedir);

  const objDir = path.join(outdir, "obj");

  if (!fs.existsSync(objDir)) {
    fs.mkdirs(objDir);
  }

  let changes = getChanges(sources, objDir);
  changes = checkHeaders(changes, headers, sources, objDir);

  let includes = include.map((i) => `-I${i}`).join(" ");
  for (let src of changes) {
    try {
      let cmd = `${compiler} -c ${src.path} -o ${src.obj} ${includes} ${objflags}`;
      console.log(cmd);
      execSync(cmd);
    } catch (err) {
      return;
    }
  }

  if (!changes.length) {
    console.log("all files up to date");
  } else {
    cmd = `${compiler} -o ${path.join(outdir, exec)} ${changes.map((c) => c.obj + " ")} ${includes} ${cflags}`;
    execSync(cmd);
  }

  fs.writeJSONSync("db.json", db);
  // console.log(config);
  // console.log(sources, changes);
};

module.exports = { buildOBJ, getObj, listFiles, checkHeaders, getChanges, buildOjbFiles };
