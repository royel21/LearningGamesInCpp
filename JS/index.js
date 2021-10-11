import { Balls, Walls, Ball, Wall, vec2 } from "./Ball.js";
import {
  CircleVsCircleCollision,
  penetrationResolver,
  collResCvsC,
  collCircleLine,
  circleVsLineResolver,
  circleVsLinePenResolver,
} from "./Solver.js";

const canvas = document.getElementById("canvas");
const ctx = canvas.getContext("2d");

const Left = 37;
const Right = 39;
const Up = 40;
const Down = 38;

let keys = [];
canvas.onkeydown = (e) => (keys[e.keyCode] = true);
canvas.onkeyup = (e) => (keys[e.keyCode] = false);

function move(b) {
  if (keys[Right]) b.acc.x = b.accel;
  if (keys[Left]) b.acc.x = -b.accel;
  if (keys[Up]) b.acc.y = b.accel;
  if (keys[Down]) b.acc.y = -b.accel;

  if (!keys[Right] && !keys[Left]) b.acc.x = 0;
  if (!keys[Up] && !keys[Down]) b.acc.y = 0;
}

let ball = new Ball(60, 60, 30, "lightblue", 1);
ball.player = true;
ball.elasticity = 0.5;

let ball2 = new Ball(120, 60, 30, "lightblue", 1);
ball2.elasticity = 1;
let ball3 = new Ball(120, 120, 30, "lightblue", 1);
ball3.elasticity = 1;
let ball4 = new Ball(240, 400, 30, "lightblue", 1);
ball4.elasticity = 1;
//
new Wall(100, 200, 400, 400);
new Wall(350, 200, 650, 200);
new Wall(150, 230, 650, 400);

let cw = canvas.clientWidth;
let ch = canvas.clientHeight;

new Wall(0, 0, cw, 0);
new Wall(cw, ch, cw, 0);
new Wall(0, ch, cw, ch);
new Wall(0, 0, 0, ch);

let x = cw / 2;
let y = ch / 2;

let vec = new vec2(x, y);

function drawLine(px, py) {
  console.log(x, y, px, py);
  let unit = vec.unit().add({ x, y }).add({ x: 56, y: 56 });
  ctx.beginPath();
  ctx.moveTo(x, y);
  ctx.lineTo(px, py);
  ctx.strokeStyle = "black";
  ctx.stroke();
}
let mpos = { x: 0, y: 0 };
canvas.addEventListener("mousemove", (e) => {
  mpos = { x: e.clientX, y: e.clientY };
});

function drawCircle() {
  ctx.beginPath();
  ctx.arc(x, y, 80, 0, 2 * Math.PI);
  ctx.strokeStyle = "black";
  ctx.stroke();
  ctx.fillStyle = "red";
  ctx.fill();
}

const mainloop = () => {
  // console.time("fps");
  ctx.clearRect(0, 0, cw, ch);
  drawCircle();
  drawLine(mpos.x, mpos.y);
  // drawLine();
  // Balls.forEach((b, i) => {
  //   if (b.player) {
  //     move(b);
  //   }

  //   for (let x = i + 1; x < Balls.length; x++) {
  //     if (CircleVsCircleCollision(Balls[i], Balls[x])) {
  //       penetrationResolver(Balls[i], Balls[x]);
  //       collResCvsC(Balls[i], Balls[x]);
  //     }
  //   }
  //   Walls.forEach((w) => {
  //     if (collCircleLine(b, w)) {
  //       circleVsLineResolver(b, w);
  //       circleVsLinePenResolver(b, w);
  //     }
  //   });
  //   b.reposition();
  //   b.display(ctx);
  //   b.draw(ctx);
  // });

  // Walls.forEach((w, i) => {
  //   w.draw(ctx);
  // });

  // console.timeEnd("fps");
  requestAnimationFrame(mainloop);
};

requestAnimationFrame(mainloop);

window.onfocus = window.onload = () => {
  canvas.focus();
};

function randInt(min, max) {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

for (let i = 0; i < 15; i++) {
  let ball = new Ball(randInt(50, 600), randInt(50, 600), randInt(10, 30), "red", randInt(1, 2));
  if (i === 0) {
    ball.setMass(0);
  }
  ball.elasticity = 3;
}
