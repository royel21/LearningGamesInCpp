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

      m.textures.push({
        id: `${ts.name}.${ex}`,
        path: `assets/textures/${name}`,
        tileWidth: ts.tileWidth,
        tileHeight: ts.tileHeight,
      });

      tilesets[ts.name] = { id: i };
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
          tileWidth: map.tileWidth,
          tileHeight: map.tileHeight,
          layer: i,
          textures: [],
          tiles: [],
        };

        let h = layer.height - 1;
        for (y = h; y > -1; y--) {
          for (x = 0; x < layer.width; x++) {
            let tile = layer.tileAt(x, y);
            let cell = layer.cellAt(x, y);

            if (tile) {
              let item = tilesets[tile.tileset.name].id;
              if (!tilemap.textures.find((t) => t.id == item)) {
                tilemap.textures.push({
                  id: item,
                  name: tile.tileset.name,
                });
              }
              item |= tile.id << 4;
              item |= cell.flippedHorizontally << 25;
              item |= cell.FlippedVertically << 26;
              item |= cell.flippedAntiDiagonally << 27;
              tilemap.tiles.push(item);

              // tilemap.tiles.push({
              //   x,
              //   y: h - y,
              //   tc: tile.id,
              //   txi: item,
              //   fx: cell.flippedHorizontally ? 1 : 0,
              //   fy: cell.FlippedVertically ? 1 : 0,
              //   r: cell.flippedAntiDiagonally ? 90 : 0,
              // });
            } else {
              tilemap.tiles.push(0);
            }
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

    return map;
  },
};

tiled.registerMapFormat("custom", customMapFormat);
