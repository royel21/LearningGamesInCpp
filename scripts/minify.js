const fs = require("fs-extra");
const path = require("path");

let dir = "assets/scenes";
for (let f of fs.readdirSync(dir)) {
  let p = path.join(dir, f);
  let file = fs.readJSONSync(p);
  fs.writeJSONSync(p, file);
}
