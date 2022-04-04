const fs = require("fs-extra");
const path = require("path");

const toClean = process.argv[2] || "";

const capitalize = (str) => {
  if (str[0]) {
    let char = str[0];
    return char.toUpperCase() + str.substr(1, str.length - 1);
  }
  return "";
};

let pdir = "./web/obj/" + toClean;
if (fs.existsSync(pdir)) {
  let files = fs.readdirSync(pdir);

  files.forEach((f) => {
    if (f.split(".").length > 1) {
      fs.removeSync(path.join(pdir, f));
      console.log(pdir, f);
    }
  });

  let PlutusO = `./web/obj/${capitalize(toClean)}.o`;
  if (fs.existsSync(PlutusO)) {
    fs.removeSync(PlutusO);
    console.log(PlutusO);
  }
}
