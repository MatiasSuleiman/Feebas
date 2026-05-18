const canvas = document.querySelector("#simulation-canvas");
const context = canvas.getContext("2d");

function resizeCanvas() {
  const bounds = canvas.getBoundingClientRect();
  const pixelRatio = window.devicePixelRatio || 1;
  const width = Math.max(1, Math.floor(bounds.width * pixelRatio));
  const height = Math.max(1, Math.floor(bounds.height * pixelRatio));

  if (canvas.width !== width || canvas.height !== height) {
    canvas.width = width;
    canvas.height = height;
  }

  drawStage(pixelRatio);
}

function drawStage(pixelRatio) {
  const width = canvas.width;
  const height = canvas.height;

  context.save();
  context.setTransform(1, 0, 0, 1, 0, 0);
  context.clearRect(0, 0, width, height);
  context.fillStyle = "#20252a";
  context.fillRect(0, 0, width, height);

  context.strokeStyle = "rgba(255, 255, 255, 0.045)";
  context.lineWidth = pixelRatio;
  context.strokeRect(pixelRatio * 0.5, pixelRatio * 0.5, width - pixelRatio, height - pixelRatio);
  context.restore();
}

window.addEventListener("resize", resizeCanvas);
resizeCanvas();

