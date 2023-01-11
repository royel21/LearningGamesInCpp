const fs = require("fs-extra");
const path = require("path");

const cfg = fs.readJsonSync("./config.json");
const pro = cfg.projects.find((p) => p.name === cfg["current-project"]);

if (pro) {
  console.log(pro.path);
}

if (pro) {
  const project = fs.readJSONSync(pro.path);

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

  const copyfromScene = (scene) => {
    console.log(scene);
    for (const items in scene) {
      for (const item of scene[items]) {
        if (items === "entities") {
          for (const comp of item.components || []) {
            if (comp.name === "Script") copyFile(comp.script);
          }
        } else if (item.path) {
          copyFile(item.path);
        }
      }
    }
  };

  for (let scene of project.scenes) {
    copyfromScene(scene);
  }
}
