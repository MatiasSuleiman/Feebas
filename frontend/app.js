const canvas = document.querySelector("#simulation-canvas");
const context = canvas.getContext("2d");
const statusElement = document.querySelector(".status");
const particleButtons = document.querySelectorAll("[data-particle]");

const colors = {
  VoidParticle: [32, 37, 42, 255],
  DirtParticle: [132, 92, 58, 255],
  GrassParticle: [91, 164, 88, 255],
  WaterParticle: [58, 139, 253, 255],
  MudParticle: [104, 70, 48, 255],
  StoneParticle: [190, 194, 198, 255],
  WoodParticle: [125, 53, 17, 255],
  FireParticle: [255, 143, 31, 255],
  TNTParticle: [255, 0, 0, 255],
};

const fireColors = [
  [255, 224, 92, 255],
  [255, 184, 48, 255],
  [255, 132, 28, 255],
  [244, 82, 24, 255],
  [255, 239, 129, 255],
];

const worldCanvas = document.createElement("canvas");
const worldContext = worldCanvas.getContext("2d");

let selectedParticle = "dirt";
let world = null;
let worldImage = null;
let isPainting = false;
let lastPaintedCell = "";
let worldRequestInFlight = false;
let worldRequestQueue = [];
let stepTimer = null;
let createBatchTimer = null;
let pendingCreateParticle = "";
const pendingCreateCells = new Set();
let fireAnimationTimer = null;
const fireCells = new Set();

function setStatus(message) {
  statusElement.textContent = message;
}

function particleLabel(particle) {
  if (particle === "grass") {
    return "Grass";
  }

  if (particle === "water") {
    return "Water";
  }

  if (particle === "mud") {
    return "Mud";
  }

  if (particle === "stone") {
    return "Stone";
  }

  if (particle === "wood") {
    return "Wood";
  }

  if (particle === "fire") {
    return "Fire";
  }


  if (particle === "TNT") {
    return "TNT";
  }

  return "Dirt";
}

function selectedLabel() {
  return particleLabel(selectedParticle);
}

function selectParticle(particle) {
  flushPendingCreateBatch();
  selectedParticle = particle;
  particleButtons.forEach((button) => {
    const isSelected = button.dataset.particle === selectedParticle;
    button.classList.toggle("is-selected", isSelected);
    button.setAttribute("aria-pressed", String(isSelected));
  });
  setStatus(`${selectedLabel()} selected`);
}

async function requestWorldApi(
  url,
  options,
  busyMessage,
  applyResponse,
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
    applyResponse(nextWorld);
    setStatus(`${selectedLabel()} selected`);
  } catch (error) {
    if (failureMessage) {
      setStatus(failureMessage);
    }
  }
}

function loadWorld() {
  requestWorldApi(
    "/world",
    {},
    "Loading world",
    applyEmptyWorld,
  );
}

function cancelScheduledStep() {
  if (stepTimer === null) {
    return;
  }

  window.clearTimeout(stepTimer);
  stepTimer = null;
}

function flushQueuedSteps() {
  worldRequestQueue = worldRequestQueue.filter((request) => request.type !== "step");
}

function queueHasRequestType(type) {
  return worldRequestQueue.some((request) => request.type === type);
}

function hasPendingCreateRequest() {
  return queueHasRequestType("create");
}

function hasPendingStepRequest() {
  return queueHasRequestType("step");
}

function hasPendingCreateBatch() {
  return pendingCreateCells.size > 0;
}

function enqueueWorldRequest(request) {
  if (request.type === "create") {
    cancelScheduledStep();
    flushQueuedSteps();
  }

  worldRequestQueue.push(request);
  drainWorldRequestQueue();
}

function drainWorldRequestQueue() {
  if (worldRequestInFlight || worldRequestQueue.length === 0) {
    return;
  }

  const request = worldRequestQueue.shift();
  worldRequestInFlight = true;

  requestWorldApi(
    request.url,
    request.options,
    request.busyMessage,
    request.applyResponse,
    request.failureMessage,
  )
    .finally(() => {
      worldRequestInFlight = false;
      drainWorldRequestQueue();

      if (worldRequestQueue.length === 0) {
        scheduleNextStep();
      }
    });
}

function scheduleNextStep() {
  if (
    stepTimer !== null ||
    worldRequestInFlight ||
    hasPendingCreateBatch() ||
    hasPendingCreateRequest() ||
    hasPendingStepRequest()
  ) {
    return;
  }

  stepTimer = window.setTimeout(() => {
    stepTimer = null;
    stepWorld({ silent: true });
  }, 16);
}

function stepWorld({ silent = false } = {}) {
  if (hasPendingCreateBatch() || hasPendingCreateRequest() || hasPendingStepRequest()) {
    return;
  }

  enqueueWorldRequest({
    type: "step",
    url: "/world/step",
    options: { method: "POST" },
    busyMessage: silent ? "" : "Stepping world",
    applyResponse: applyPatchResponse,
    failureMessage: silent ? "" : "API request failed",
  });
}

function createParticleAt(cell) {
  if (pendingCreateParticle && pendingCreateParticle !== selectedParticle) {
    flushPendingCreateBatch();
  }

  cancelScheduledStep();
  flushQueuedSteps();
  pendingCreateParticle = selectedParticle;
  pendingCreateCells.add(cellKey(cell.x, cell.y));

  if (createBatchTimer === null) {
    createBatchTimer = window.setTimeout(() => {
      createBatchTimer = null;
      flushPendingCreateBatch();
    }, 16);
  }
}

function flushPendingCreateBatch() {
  if (createBatchTimer !== null) {
    window.clearTimeout(createBatchTimer);
    createBatchTimer = null;
  }

  if (pendingCreateCells.size === 0) {
    pendingCreateParticle = "";
    return;
  }

  const particle = pendingCreateParticle || selectedParticle;
  const paths = {
    dirt: "/world/create-dirt-particles",
    grass: "/world/create-grass-particles",
    water: "/world/create-water-particles",
    mud: "/world/create-mud-particles",
    stone: "/world/create-stone-particles",
    wood: "/world/create-wood-particles",
    fire: "/world/create-fire-particles",
    TNT: "/world/create-TNT-particles",
  };
  const path = paths[particle] || paths.dirt;
  const cells = Array.from(pendingCreateCells).join(",");

  pendingCreateCells.clear();
  pendingCreateParticle = "";

  enqueueWorldRequest({
    type: "create",
    url: `${path}?cells=${cells}`,
    options: { method: "POST" },
    busyMessage: `Creating ${particleLabel(particle)}`,
    applyResponse: applyPatchResponse,
    failureMessage: "API request failed",
  });
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

  drawVisibleCanvas();
}

function worldHasFire() {
  return fireCells.size > 0;
}

function scheduleFireAnimation() {
  if (fireAnimationTimer !== null || !worldHasFire()) {
    return;
  }

  fireAnimationTimer = window.setTimeout(() => {
    fireAnimationTimer = null;
    animateFireCells();
  }, 90);
}

function fireColorAt(x, y, time) {
  const paletteIndex = Math.floor(time / 90 + x * 7 + y * 13) % fireColors.length;
  return fireColors[paletteIndex];
}

function cellKey(x, y) {
  return `${x}:${y}`;
}

function cellIndex(x, y) {
  return y * world.width + x;
}

function imageIndexForCell(x, y) {
  const renderY = world.height - 1 - y;
  return (renderY * world.width + x) * 4;
}

function trackFireCell(x, y, particleType) {
  const key = cellKey(x, y);
  if (particleType === "FireParticle") {
    fireCells.add(key);
  } else {
    fireCells.delete(key);
  }
}

function setCellPixel(x, y, particleType, time = performance.now()) {
  if (!worldImage) {
    return;
  }

  const imageIndex = imageIndexForCell(x, y);
  const color = particleType === "FireParticle"
    ? fireColorAt(x, y, time)
    : colors[particleType] || colors.VoidParticle;

  worldImage.data[imageIndex] = color[0];
  worldImage.data[imageIndex + 1] = color[1];
  worldImage.data[imageIndex + 2] = color[2];
  worldImage.data[imageIndex + 3] = color[3];
}

function applyEmptyWorld(worldMetadata) {
  world = {
    width: worldMetadata.width,
    height: worldMetadata.height,
    cells: Array(worldMetadata.width * worldMetadata.height).fill("VoidParticle"),
  };
  fireCells.clear();

  if (worldCanvas.width !== world.width || worldCanvas.height !== world.height) {
    worldCanvas.width = world.width;
    worldCanvas.height = world.height;
  }

  worldImage = worldContext.createImageData(world.width, world.height);
  const now = performance.now();

  for (let y = 0; y < world.height; y += 1) {
    for (let x = 0; x < world.width; x += 1) {
      setCellPixel(x, y, "VoidParticle", now);
    }
  }

  worldContext.putImageData(worldImage, 0, 0);
  drawVisibleCanvas();
  scheduleFireAnimation();
  scheduleNextStep();
}

function applyPatchResponse(patch) {
  if (!world || !worldImage) {
    loadWorld();
    return;
  }

  if (
    typeof patch.revision === "number" &&
    typeof world.revision === "number" &&
    patch.revision !== world.revision + 1
  ) {
    loadWorld();
    return;
  }

  if (typeof patch.revision === "number") {
    world.revision = patch.revision;
  }

  const changes = Array.isArray(patch.changes) ? patch.changes : [];
  const now = performance.now();

  changes.forEach((change) => {
    if (
      change.x < 0 ||
      change.y < 0 ||
      change.x >= world.width ||
      change.y >= world.height
    ) {
      return;
    }

    const particleType = change.particle || "VoidParticle";
    world.cells[cellIndex(change.x, change.y)] = particleType;
    trackFireCell(change.x, change.y, particleType);
    setCellPixel(change.x, change.y, particleType, now);
  });

  if (changes.length > 0) {
    worldContext.putImageData(worldImage, 0, 0);
    drawVisibleCanvas();
  }

  scheduleFireAnimation();
}

function animateFireCells() {
  if (!world || !worldImage || !worldHasFire()) {
    return;
  }

  const now = performance.now();
  fireCells.forEach((key) => {
    const separatorIndex = key.indexOf(":");
    const x = Number(key.slice(0, separatorIndex));
    const y = Number(key.slice(separatorIndex + 1));
    setCellPixel(x, y, "FireParticle", now);
  });

  worldContext.putImageData(worldImage, 0, 0);
  drawVisibleCanvas();
  scheduleFireAnimation();
}

function drawVisibleCanvas() {
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
  flushPendingCreateBatch();
  canvas.releasePointerCapture(event.pointerId);
});

canvas.addEventListener("pointercancel", () => {
  isPainting = false;
  lastPaintedCell = "";
  flushPendingCreateBatch();
});

window.addEventListener("resize", resizeCanvas);

selectParticle(selectedParticle);
resizeCanvas();
loadWorld();
