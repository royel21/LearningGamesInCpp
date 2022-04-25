//https://www.mapeditor.org/docs/scripting/classes/Tile.html
let tilesets = [];

function encode({ x, y, fx, fy, r }) {
  let tile = x | (y << 12) | (r << 20) | (fx << 21) | (fy << 22);
  return tile;
}

function encodeTex({ i, tex }) {
  return i | (tex << 12);
}

var customMapFormat = {
  name: "Plutus Json",
  extension: "json",

  write: function (map, fileName) {
    var m = {
      format: "Plutus",
      tilesets: [],
      layers: [],
    };

    for (var i = 0; i < map.tilesets.length; i++) {
      var ts = map.tilesets[i];
      tiled.log(ts.nextTileId);
      m.tilesets.push({
        id: i,
        name: ts.name,
        image: ts.image,
        tileWidth: ts.tileWidth,
        tileHeight: ts.tileHeight,
      });

      tilesets[ts.name] = { id: i };
    }

    for (var i = 0; i < map.layerCount; ++i) {
      var layer = map.layerAt(i);
      if (layer.isTileLayer) {
        var tiles = [];
        let h = layer.height - 1;
        for (y = h; y > -1; y--) {
          for (x = 0; x < layer.width; x++) {
            let tile = layer.tileAt(x, y);
            let cell = layer.cellAt(x, y);
            if (tile) {
              let id = tilesets[tile.tileset.name].id;
              tiled.log(
                `${x}, ${h - y}, ${id}, ${cell.flippedAntiDiagonally} ${cell.flippedHorizontally}, ${
                  cell.flippedVertically
                }`
              );

              tiles.push(
                encode({
                  x,
                  y: h - y,
                  fx: cell.flippedHorizontally,
                  fy: cell.FlippedVertically,
                  r: cell.flippedAntiDiagonally,
                }),
                encodeTex({
                  i: id,
                  tex: tile.id,
                })
              );
            }
          }
        }
        m.layers.push({
          type: 0,
          name: layer.name,
          width: layer.width,
          height: layer.height,
          data: tiles,
        });
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
