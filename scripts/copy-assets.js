const fs = require("fs-extra");
const path = require("path");

const cmd = process.argv[2];

path.join("ZombiesGame", "ZombiesGame.json");

if (cmd) {
  const project = fs.readJSONSync(path.join("ZombiesGame", "assets", "scenes", cmd));

  const copyFile = (file) => {
    try {
      let dir = path.dirname(file);

      dir = path.join("web/", path.normalize(dir));

      if (!fs.existsSync(dir)) {
        fs.mkdirsSync(dir);
      }

      if (fs.existsSync(file)) {
        fs.copySync(file, "web/" + path.normalize(file));
      }
    } catch (error) {
      console.log(error);
    }
  };

  for (const items in project) {
    for (const item of project[items]) {
      if (items === "entities") {
        for (const comp of item.components || []) {
          if (comp.name === "Script") copyFile(comp.script);
        }
      } else if (item.path) {
        copyFile(item.path);
      }
    }
  }
}
