import { vec2 } from "./Ball.js";

export function CircleVsCircleCollision(b1, b2) {
  //if the sum of the radius is equal or bigger than the mangnitud of the diference of b2 - b2 is it colliding
  return b1.r + b2.r >= b2.pos.subtr(b1.pos).mag();
}

export function penetrationResolver(b1, b2) {
  let dist = b1.pos.subtr(b2.pos);
  //Penetration Depth
  let penDepth = b1.r + b2.r - dist.mag();
  //   Penetration Distance
  let penRes = dist.unit().mul(penDepth / (b1.invMass + b2.invMass));

  let pen = penRes.mul(b1.invMass);
  if (pen.x > b1.r || pen.y > b1.r) {
    pen.x = pen.x > 0 ? b1.r : -b1.r;
    pen.y = pen.y > 0 ? b1.r : -b1.r;
  }

  let pen2 = penRes.mul(-b1.invMass);
  if (pen2.x > b2.r || pen2.y > b2.r) {
    pen2.x = pen2.x > 0 ? b2.r : -b2.r;
    pen2.y = pen2.y > 0 ? b2.r : -b2.r;
  }

  b1.pos = b1.pos.add(pen);
  b2.pos = b2.pos.add(pen2);
}

export function collResCvsC(b1, b2) {
  let normal = b1.pos.subtr(b2.pos).unit();
  //relative velocity
  let relVel = b1.vel.subtr(b2.vel);
  //Separation Velocity
  let sepVel = vec2.dot(relVel, normal);
  //new separation velocity
  let newSepVel = -sepVel * Math.min(b1.elasticity, b2.elasticity);

  // let sepVelVec = normal.mul(newSepVel);
  let vsepDiff = newSepVel - sepVel;
  let impulse = vsepDiff / (b1.invMass + b2.invMass);
  let impulseVec = normal.mul(impulse);

  b1.vel = b1.vel.add(impulseVec.mul(b1.invMass));
  b2.vel = b2.vel.add(impulseVec.mul(-b2.invMass));
}

/************************************************************** */
export function closePointCircleLine(circle, line) {
  let line2CircleStart = line.start.subtr(circle.pos);

  if (vec2.dot(line.unit(), line2CircleStart) > 0) {
    return line.start;
  }

  let line2CircleEnd = circle.pos.subtr(line.end);
  if (vec2.dot(line.unit(), line2CircleEnd) > 0) {
    return line.end;
  }

  let closestDist = vec2.dot(line.unit(), line2CircleStart);

  let closestVec = line.unit().mul(closestDist);

  return line.start.subtr(closestVec);
}
/************************************************************** */
export function collCircleLine(circle, line) {
  let circle2ClosestPoint = closePointCircleLine(circle, line).subtr(circle.pos);

  return circle2ClosestPoint.mag() <= circle.r;
}

export function circleVsLineResolver(circle, line) {
  let penVec = circle.pos.subtr(closePointCircleLine(circle, line));

  circle.pos = circle.pos.add(penVec.unit().mul(circle.r - penVec.mag()));
}

export function circleVsLinePenResolver(circle, line) {
  let normal = circle.pos.subtr(closePointCircleLine(circle, line)).unit();

  let separationVel = vec2.dot(circle.vel, normal);
  let newVel = -separationVel * 0;
  let velSepDiff = separationVel - newVel;
  let pen2 = normal.mul(0);
  circle.vel = n;
}
