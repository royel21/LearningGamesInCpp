function setBallVerticesAlongAxis(obj, axis) {
  if (obj instanceof Ball) {
    obj.vertex[0] = obj.pos.add(axis.unit().mult(-obj.r));
    obj.vertex[1] = obj.pos.add(axis.unit().mult(obj.r));
  }
}

//the ball vertices always need to be recalculated based on the current projection axis direction
function setBallVerticesAlongAxis(obj, axis) {
  if (obj instanceof Ball) {
    obj.vertex[0] = obj.pos.add(axis.unit().mult(-obj.r));
    obj.vertex[1] = obj.pos.add(axis.unit().mult(obj.r));
  }
}

function projShapeOntoAxis(axis, obj) {
  setBallVerticesAlongAxis(obj, axis);
  let min = Vector.dot(axis, obj.vertex[0]);
  let max = min;
  let collVertex = obj.vertex[0];
  for (let i = 0; i < obj.vertex.length; i++) {
    let p = Vector.dot(axis, obj.vertex[i]);
    if (p < min) {
      min = p;
      collVertex = obj.vertex[i];
    }
    if (p > max) {
      max = p;
    }
  }
  return {
    min: min,
    max: max,
    collVertex: collVertex,
  };
}

//applying the separating axis theorem on two objects
function sat(o1, o2) {
  let minOverlap = null;
  let smallestAxis;
  let vertexObj;

  let axes = findAxes(o1, o2);
  let proj1,
    proj2 = 0;
  let firstShapeAxes = getFirstShapeAxes(o1);

  for (let i = 0; i < axes.length; i++) {
    proj1 = projShapeOntoAxis(axes[i], o1);
    proj2 = projShapeOntoAxis(axes[i], o2);
    let overlap = Math.min(proj1.max, proj2.max) - Math.max(proj1.min, proj2.min);
    if (overlap < 0) {
      return false;
    }

    if ((proj1.max > proj2.max && proj1.min < proj2.min) || (proj1.max < proj2.max && proj1.min > proj2.min)) {
      let mins = Math.abs(proj1.min - proj2.min);
      let maxs = Math.abs(proj1.max - proj2.max);
      if (mins < maxs) {
        overlap += mins;
      } else {
        overlap += maxs;
        axes[i] = axes[i].mult(-1);
      }
    }

    if (overlap < minOverlap || minOverlap === null) {
      minOverlap = overlap;
      smallestAxis = axes[i];
      if (i < firstShapeAxes) {
        vertexObj = o2;
        if (proj1.max > proj2.max) {
          smallestAxis = axes[i].mult(-1);
        }
      } else {
        vertexObj = o1;
        if (proj1.max < proj2.max) {
          smallestAxis = axes[i].mult(-1);
        }
      }
    }
  }

  let contactVertex = projShapeOntoAxis(smallestAxis, vertexObj).collVertex;
  smallestAxis.drawVec(contactVertex.x, contactVertex.y, minOverlap, "blue");

  return true;
}
