const canvas = document.getElementById("canvas");
const ctx = canvas.getContext("2d");

const Dec2Rag = (dec) => (Math.PI / 180) * dec;

let x = 100;
let y = 100;
let keys = [];

class vec {
  constructor(x, y) {
    this.x = x || 0;
    this.y = y || 0;
  }
  add(v) {
    return new vec(this.x + v.x, this.y + v.y);
  }
  subtr(v) {
    return new vec(this.x - v.x, this.y - v.y);
  }

  mul(n) {
    return new vec(this.x * n, this.y * n);
  }
  mag() {
    return Math.sqrt(this.x ** 2, this.y ** 2);
  }
}

/*************************************************************** */
canvas.onkeydown = (e) => {
  keys[e.keyCode] = true;
};

canvas.onkeyup = (e) => {
  keys[e.keyCode] = false;
};

function move(b) {
  if (keys[37]) {
    b.acc.x = -b.accel;
  }

  if (keys[39]) {
    b.acc.x = b.accel;
  }

  if (keys[38]) {
    b.acc.y = -b.accel;
  }

  if (keys[40]) {
    b.acc.y = b.accel;
  }

  if (!keys[38] && !keys[40]) b.acc.y = 0;
  if (!keys[39] && !keys[37]) b.acc.x = 0;

  b.vel = b.vel.add(b.acc);

  b.vel = b.vel.mul(1 - b.fricction);

  b.pos = b.pos.add(b.vel);
}

let Balls = [];

class Ball {
  constructor(x, y, r, color) {
    this.pos = new vec(x, y);
    this.r = r;
    this.ax = 0;
    this.acc = new vec();
    this.vel = new vec();
    this.accel = 1;
    this.fricction = 0.1;
    this.Player = false;
    this.color = color;
    Balls.push(this);
  }
  draw() {
    ctx.beginPath();
    ctx.arc(this.pos.x, this.pos.y, this.r, 0, 2 * Math.PI);
    ctx.strokeStyle = "black";
    ctx.stroke();
    ctx.fillStyle = this.color;
    ctx.fill();
  }
  display() {
    ctx.beginPath();
    ctx.moveTo(this.pos.x, this.pos.y);
    ctx.lineTo(this.pos.x + this.acc.x * 100, this.pos.y + this.acc.y * 100);
    ctx.strokeStyle = "green";
    ctx.stroke();

    ctx.beginPath();
    ctx.moveTo(this.pos.x, this.pos.y);
    ctx.lineTo(this.pos.x + this.vel.x * 10, this.pos.y + this.vel.y * 10);
    ctx.strokeStyle = "blue ";
    ctx.stroke();
  }
}
/*************************************************************** */

let ball1 = new Ball(200, 200, 30, "red");

function mainloop() {
  move(ball1);
  ctx.clearRect(0, 0, canvas.clientWidth, canvas.clientHeight);
  for (let ball of Balls) {
    ball.draw();
    ball.display();
  }
  requestAnimationFrame(mainloop);
}

requestAnimationFrame(mainloop);

window.onfocus = () => {
  canvas.focus();
};
