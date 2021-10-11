export class vec2 {
  constructor(x, y) {
    this.x = x;
    this.y = y;
  }

  add(v) {
    return new vec2(this.x + v.x, this.y + v.y);
  }

  subtr(v) {
    return new vec2(this.x - v.x, this.y - v.y);
  }

  mul(n) {
    return new vec2(this.x * n, this.y * n);
  }

  mag() {
    return Math.sqrt(this.x * this.x + this.y * this.y);
  }

  unit() {
    let mag = this.mag();
    return mag ? new vec2(this.x / mag, this.y / mag) : new vec2(0, 0);
  }

  normal() {
    return new vec2(-this.y, this.x).unit();
  }

  static dot(v1, v2) {
    return v1.x * v2.x + v1.y * v2.y;
  }

  draw(ctx, startX, startY, n, c) {
    if (ctx) {
      ctx.beginPath();
      ctx.moveTo(startX, startY);
      ctx.lineTo(startX + this.x * n, startY + this.y * n);
      ctx.strokeStyle = c;
      ctx.stroke();
    }
  }
}

export let Balls = [];
export let Walls = [];

export class Ball {
  constructor(x, y, r, c, m) {
    this.pos = new vec2(x, y);
    this.vel = new vec2(0, 0);
    this.acc = new vec2(0, 0);
    this.r = r;
    this.c = c;
    this.mass = m;
    this.invMass = m ? 1 / m : 0;

    this.accel = 1;
    this.fricction = 0.05;
    this.elasticity = 1;

    this.player = false;
    Balls.push(this);
  }

  setMass(val) {
    this.mass = val;
    this.invMass = val ? 1 / val : 0;
  }
  draw(ctx) {
    ctx.beginPath();
    ctx.arc(this.pos.x, this.pos.y, this.r, 0, 2 * Math.PI);
    ctx.strokeStyle = "black";
    ctx.stroke();
    if (this.c) {
      ctx.fillStyle = this.c;
      ctx.fill();
    }
  }
  display(ctx) {
    this.vel.draw(ctx, this.pos.x, this.pos.y, 10, "blue");
    ctx.fillStyle = "black";
    ctx.fillText("m: " + this.mass, this.pos.x - 10, this.pos.y - 5);
    ctx.fillText("e: " + this.elasticity, this.pos.x - 10, this.pos.y + 5);
    ctx.fillText("vel: " + this.vel.x?.toFixed(2) + "," + this.vel.y?.toFixed(2), this.pos.x - 10, this.pos.y - 36);
  }

  reposition() {
    this.acc = this.acc.unit().mul(this.accel);
    this.vel = this.vel.add(this.acc);
    this.vel = this.vel.mul(1 - this.fricction);
    this.pos = this.pos.add(this.vel);
  }
}

export class Wall {
  constructor(x1, y1, x2, y2) {
    this.start = new vec2(x1, y1);
    this.end = new vec2(x2, y2);
    Walls.push(this);
  }

  draw(ctx) {
    ctx.beginPath();
    ctx.moveTo(this.start.x, this.start.y);
    ctx.lineTo(this.end.x, this.end.y);
    ctx.strokeStyle = "black";
    ctx.stroke();
  }

  unit() {
    return this.end.subtr(this.start).unit();
  }
}

export default Ball;
