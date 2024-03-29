var statusElement = document.getElementById("status");
var progressElement = document.getElementById("progress");
var spinnerElement = document.getElementById("spinner");

var Module = {
  preRun: [],
  postRun: [],
  print: "",
  canvas: "",
  setStatus: "",
  totalDependencies: 0,
  monitorRunDependencies: "",
};

Module.monitorRunDependencies = function (left) {
  this.totalDependencies = Math.max(this.totalDependencies, left);
  Module.setStatus(
    left
      ? "Preparing... (" + (this.totalDependencies - left) + "/" + this.totalDependencies + ")"
      : "All downloads complete."
  );
};

Module.print = function (text) {
  if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(" ");
  console.log(text);
};

Module.canvas = (function () {
  var canvas = document.getElementById("canvas");

  // As a default initial behavior, pop up an alert when webgl context is lost. To make your
  // application robust, you may want to override this behavior before shipping!
  // See http://www.khronos.org/registry/webgl/specs/latest/1.0/#5.15.2
  canvas.addEventListener(
    "webglcontextlost",
    function (e) {
      alert("WebGL context lost. You will need to reload the page.");
      e.preventDefault();
    },
    false
  );

  return canvas;
})();

Module.setStatus = function (text) {
  if (!Module.setStatus.last) Module.setStatus.last = { time: Date.now(), text: "" };
  if (text === Module.setStatus.last.text) return;
  var m = text.match(/([^(]+)\((\d+(\.\d+)?)\/(\d+)\)/);
  var now = Date.now();
  if (m && now - Module.setStatus.last.time < 30) return; // if this is a progress update, skip it if too soon
  Module.setStatus.last.time = now;
  Module.setStatus.last.text = text;
  if (m) {
    text = m[1];
    progressElement.value = parseInt(m[2]) * 100;
    progressElement.max = parseInt(m[4]) * 100;
    progressElement.hidden = false;
    spinnerElement.hidden = false;
  } else {
    progressElement.value = null;
    progressElement.max = null;
    progressElement.hidden = true;
    if (!text) spinnerElement.hidden = true;
  }
  statusElement.innerHTML = text;
};

Module.setStatus("Downloading...");

window.onerror = function () {
  Module.setStatus("Exception thrown, see JavaScript console");
  spinnerElement.style.display = "none";
  Module.setStatus = function (text) {
    if (text) console.error("[post-exception status] " + text);
  };
};
