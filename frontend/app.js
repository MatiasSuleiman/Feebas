const canvas = document.querySelector("#simulation-canvas");
const context = canvas.getContext("2d");
const statusElement = document.querySelector(".status");
const particleButtons = document.querySelectorAll("[data-particle]");
const stepButton = document.querySelector("[data-action='step']");

const colors = {
  VoidParticle: [32, 37, 42, 255],
  DirtParticle: [132, 92, 58, 255],
  GrassParticle: [91, 164, 88, 255],
  WaterParticle: [58, 139, 253, 255],
  MudParticle: [104, 70, 48, 255],
  StoneParticle: [190, 194, 198, 255],
  WoodParticle: [125, 53, 17, 255],
};

const worldCanvas = document.createElement("canvas");
const worldContext = worldCanvas.getContext("2d");

let selectedParticle = "dirt";
let world = null;
let isPainting = false;
let lastPaintedCell = "";
let inputGeneration = 0;
let stepInFlight = false;

function setStatus(message) {
  statusElement.textContent = message;
}

function selectedLabel() {
  if (selectedParticle === "grass") {
    return "Grass";
  }

  if (selectedParticle === "water") {
    return "Water";
  }

  if (selectedParticle === "mud") {
    return "Mud";
  }

  if (selectedParticle === "stone") {
    return "Stone";
  }

  if (selectedParticle === "wood") {
    return "Wood";
  }
  return "Dirt";
}

function selectParticle(particle) {
  selectedParticle = particle;
  particleButtons.forEach((button) => {
    const isSelected = button.dataset.particle === selectedParticle;
    button.classList.toggle("is-selected", isSelected);
    button.setAttribute("aria-pressed", String(isSelected));
  });
  setStatus(`${selectedLabel()} selected`);
}

async function callWorldApi(
  url,
  options,
  busyMessage,
  shouldApplyResponse = () => true,
  failureMessage = "API request failed",
) {
  if (busyMessage) {
    setStatus(busyMessage);
  }

  try {
    const response = await fetch(url, options);
    if (!response.ok) {
      throw new Error(`Request failed with ${response.status}`);
    }

    const nextWorld = await response.json();
    if (!shouldApplyResponse()) {
      return;
    }

    world = nextWorld;
    drawWorld();
    setStatus(`${selectedLabel()} selected`);
  } catch (error) {
    if (failureMessage) {
      setStatus(failureMessage);
    }
  }
}

function loadWorld() {
  const generation = inputGeneration;
  callWorldApi("/world", {}, "Loading world", () => generation === inputGeneration);
}

function stepWorld({ silent = false } = {}) {
  if (stepInFlight) {
    return;
  }

  stepInFlight = true;
  const generation = inputGeneration;
  callWorldApi(
    "/world/step",
    { method: "POST" },
    silent ? "" : "Stepping world",
    () => generation === inputGeneration,
    silent ? "" : "API request failed",
  )
    .finally(() => {
      stepInFlight = false;
    });
}

function createParticleAt(cell) {
  const paths = {
    dirt: "/world/create-dirt-particle-at",
    grass: "/world/create-grass-particle-at",
    water: "/world/create-water-particle-at",
    mud: "/world/create-mud-particle-at",
    stone: "/world/create-stone-particle-at",
    wood: "/world/create-wood-particle-at",
  };
  const path = paths[selectedParticle] || paths.dirt;
  const url = `${path}?x=${encodeURIComponent(cell.x)}&y=${encodeURIComponent(cell.y)}`;
  inputGeneration += 1;
  callWorldApi(url, { method: "POST" }, `Creating ${selectedLabel()}`);
}

function resizeCanvas() {
  const bounds = canvas.getBoundingClientRect();
  const pixelRatio = window.devicePixelRatio || 1;
  const width = Math.max(1, Math.floor(bounds.width * pixelRatio));
  const height = Math.max(1, Math.floor(bounds.height * pixelRatio));

  if (canvas.width !== width || canvas.height !== height) {
    canvas.width = width;
    canvas.height = height;
  }

  drawWorld();
}

function drawWorld() {
  context.save();
  context.setTransform(1, 0, 0, 1, 0, 0);
  context.imageSmoothingEnabled = false;
  context.clearRect(0, 0, canvas.width, canvas.height);

  if (!world) {
    context.fillStyle = "#20252a";
    context.fillRect(0, 0, canvas.width, canvas.height);
    drawBorder();
    context.restore();
    return;
  }

  if (worldCanvas.width !== world.width || worldCanvas.height !== world.height) {
    worldCanvas.width = world.width;
    worldCanvas.height = world.height;
  }

  const image = worldContext.createImageData(world.width, world.height);
  for (let worldY = 0; worldY < world.height; worldY += 1) {
    for (let worldX = 0; worldX < world.width; worldX += 1) {
      const worldIndex = worldY * world.width + worldX;
      const renderY = world.height - 1 - worldY;
      const imageIndex = (renderY * world.width + worldX) * 4;
      const color = colors[world.cells[worldIndex]] || colors.VoidParticle;

      image.data[imageIndex] = color[0];
      image.data[imageIndex + 1] = color[1];
      image.data[imageIndex + 2] = color[2];
      image.data[imageIndex + 3] = color[3];
    }
  }

  worldContext.putImageData(image, 0, 0);
  context.drawImage(worldCanvas, 0, 0, canvas.width, canvas.height);
  drawBorder();
  context.restore();
}

function drawBorder() {
  const pixelRatio = window.devicePixelRatio || 1;
  context.strokeStyle = "rgba(255, 255, 255, 0.045)";
  context.lineWidth = pixelRatio;
  context.strokeRect(pixelRatio * 0.5, pixelRatio * 0.5, canvas.width - pixelRatio, canvas.height - pixelRatio);
}

function canvasToWorldCell(event) {
  if (!world) {
    return null;
  }

  const bounds = canvas.getBoundingClientRect();
  const relativeX = (event.clientX - bounds.left) / bounds.width;
  const relativeY = (event.clientY - bounds.top) / bounds.height;

  return {
    x: Math.min(world.width - 1, Math.max(0, Math.floor(relativeX * world.width))),
    y: Math.min(world.height - 1, Math.max(0, world.height - 1 - Math.floor(relativeY * world.height))),
  };
}

function createParticleFromPointer(event) {
  const cell = canvasToWorldCell(event);
  if (!cell) {
    return;
  }

  const cellKey = `${cell.x}:${cell.y}`;
  if (cellKey === lastPaintedCell) {
    return;
  }

  lastPaintedCell = cellKey;
  createParticleAt(cell);
}

particleButtons.forEach((button) => {
  button.addEventListener("click", () => selectParticle(button.dataset.particle));
});

stepButton.addEventListener("click", stepWorld);

canvas.addEventListener("pointerdown", (event) => {
  if (event.button !== 0) {
    return;
  }

  isPainting = true;
  lastPaintedCell = "";
  canvas.setPointerCapture(event.pointerId);
  createParticleFromPointer(event);
});

canvas.addEventListener("pointermove", (event) => {
  if (isPainting) {
    createParticleFromPointer(event);
  }
});

canvas.addEventListener("pointerup", (event) => {
  isPainting = false;
  lastPaintedCell = "";
  canvas.releasePointerCapture(event.pointerId);
});

canvas.addEventListener("pointercancel", () => {
  isPainting = false;
  lastPaintedCell = "";
});

window.addEventListener("resize", resizeCanvas);

selectParticle(selectedParticle);
resizeCanvas();
loadWorld();
setInterval(() => stepWorld({ silent: true }), 5);
