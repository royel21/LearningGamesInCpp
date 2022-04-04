const fs = require("fs-extra");
const path = require("path");

const project = fs.readJSONSync("assets/scenes/Physics.json");

// console.log(project);

const copyFile = (file) => {
  try {
    let dir = path.dirname(file);
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

if (project.textures) {
  for (const file of project.textures) {
    copyFile(file.path);
  }
}

if (project.entities) {
  for (const ent of project.entities) {
    for (const comp of ent.components || []) {
      if (comp.name === "Script") copyFile(comp.script);
    }
  }
}
