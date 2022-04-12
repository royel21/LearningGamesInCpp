const { execSync } = require("child_process");
const crypto = require("crypto");

const fs = require("fs-extra");
const path = require("path");

let db = { files: {}, headers: {} };

const dbPath = path.join(path.dirname(__filename), "db.json");

if (fs.existsSync(dbPath)) {
  db = fs.readJSONSync(dbPath);
}

const getArray = (d) => (d instanceof Array ? d : [d]);

const getLastMod = (file) => {
  if (fs.existsSync(file)) {
    return crypto
      .createHash("md5")
      .update(fs.readFileSync(file, { encoding: "utf-8" }))
      .digest("hex");
  }

  return 0;
};

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

const checkHeaders = (files, headers, sources) => {
  let srcs = sources.filter((f1) => !files.find((f2) => f2 === f1));
  if (srcs.length === 0) return files;

  for (const h of headers) {
    let lastMod = fs.statSync(h).mtime.getTime() - 2000;
    if (db.headers[h] !== lastMod) {
      db.headers[h] = lastMod;
      let name = path.basename(h);
      for (let i = srcs.length - 1; i > 0; i--) {
        const src = srcs[i];

        const content = fs.readFileSync(src, { encoding: "utf-8" });
        if (content.includes(name)) {
          files.push(src);
          srcs = srcs.filter((s) => s != src);
        }
      }
    }

    if (srcs.length === 0) break;
  }
  return files;
};

const getChanges = (files, outdir) => {
  let changes = [];
  for (const file of files) {
    let lastmod = getLastMod(file);

    if (lastmod !== db.files[file] || !fs.existsSync(getObj(outdir, file))) {
      db.files[file] = lastmod;
      changes.push(file);
    }
  }

  return changes;
};

const buildOjbFiles = ({ compiler, include, outdir, files, cflags, rebuild, sync }) => {
  let includes = include.map((i) => `-I${i}`).join(" ");

  if (!fs.existsSync(outdir)) {
    fs.mkdirs(outdir);
  }
  const objects = files.map((f) => ({ filePath: f, objPath: getObj(outdir, f) }));

  let fileWriten = false;

  for (let src of objects) {
    try {
      let cmd = `${compiler} -c ${src.filePath} -o ${src.objPath} ${includes} ${cflags}`;
      console.log(cmd);
      execSync(cmd);
      fileWriten = true;
    } catch (err) {
      return [];
    }
  }

  if (fileWriten && sync) fs.writeJSONSync(dbPath, db);
};

module.exports = { getObj, listFiles, checkHeaders, getChanges, buildOjbFiles, db, getLastMod };
