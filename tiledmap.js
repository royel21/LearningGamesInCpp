//create hard link to tiled extension folder
//mklink /h C:\Users\rconsoro\AppData\Local\Tiled\extensions\plutus.js tiledmap.js

let tilesets = [];

var customMapFormat = {
  name: "Plutus Json",
  extension: "json",

  write: function (map, fileName) {
    var m = {
      type: 1,
      format: "Plutus",
      textures: [],
      entities: [],
    };

    for (var i = 0; i < map.tilesets.length; i++) {
      var ts = map.tilesets[i];
      let name = FileInfo.fileName(ts.image);
      let ex = name.split(".").pop();
      let nameex = `${ts.name}.${ex}`;
      m.textures.push({
        id: nameex,
        path: `assets/textures/${name}`,
        tileWidth: ts.tileWidth,
        tileHeight: ts.tileHeight,
        spacing: ts.tileSpacing,
        margin: ts.margin,
      });

      tilesets[ts.name] = { id: i, name: nameex };
    }

    tiled.log("testing2");

    for (var i = 0; i < map.layerCount; ++i) {
      var layer = map.layerAt(i);
      if (layer.isTileLayer) {
        let entity = {
          name: layer.name,
          components: [],
        };

        let tilemap = {
          name: "TileMap",
          width: map.width,
          height: map.height,
          tileWidth: map.tileWidth,
          tileHeight: map.tileHeight,
          layer: i,
          textures: [],
          animateTiles: [],
          animations: [],
          tiles: [],
        };

        let h = layer.height - 1;
        let index = 0;
        let animIndex = 0;
        for (y = h; y > -1; y--) {
          for (x = 0; x < layer.width; x++) {
            let tile = layer.tileAt(x, y);
            let cell = layer.cellAt(x, y);

            if (tile) {
              let tex = tilesets[tile.tileset.name];
              if (!tilemap.textures.find((t) => t.id == tex.id)) {
                tilemap.textures.push(tex);
              }

              if (tile.animated) {
                let found = tilemap.animations.find((t) => t.id == tile.id);

                if (!found) {
                  found = {
                    index: animIndex++,
                    id: tile.id,
                    texId: tex.id,
                    duration: tile.frames[0].duration / 1000.0,
                    frames: tile.frames.map((f) => f.tileId),
                  };
                  tilemap.animations.push(found);
                }

                // tiled.log(`ani: ${found.id} ${found.index}, inx: ${index}, inx: ${(index << 12) | found.id} ${x} ${y}`);
                tilemap.animateTiles.push((index << 12) | found.index);

                tilemap.tiles.push(0);
              } else {
                let item = tex.id + 1;
                item |= tile.id << 4;
                item |= cell.flippedHorizontally << 25;
                item |= cell.FlippedVertically << 26;
                item |= cell.flippedAntiDiagonally << 27;
                tilemap.tiles.push(item);
              }
            } else {
              tilemap.tiles.push(0);
            }
            index++;
          }
        }
        entity.components.push(tilemap);
        m.entities.push(entity);
      }

      if (layer.isImageLayer) {
      }

      if (layer.isObjectLayer) {
      }
    }

    var file = new TextFile(fileName, TextFile.WriteOnly);
    file.write(JSON.stringify(m));
    file.commit();
  },

  read: function (fileName) {
    var file = new TextFile(fileName, TextFile.ReadOnly);
    var data = file.readAll();
    var m = JSON.parse(data);
    file.close();

    var map = new TileMap();
    map.infinite = false;

    map.width = m.width;
    map.height = m.height;

    for (let ent of m.entities) {
      if (ent && ent.components) {
        let tilemap = ent.components.fint((c) => c.name === "TileMap");
        if (tilemap) {
        }
      }
    }
    return map;
  },
};

tiled.registerMapFormat("custom", customMapFormat);
