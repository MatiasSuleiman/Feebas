# Project Instructions

This project is a tiny particle simulator webapp. Keep the implementation small,
local-first, and easy to inspect.

## Architecture

- Use vanilla HTML, CSS, and JavaScript for the frontend.
- Render the simulation with an HTML `canvas` and redraw it from JavaScript.
- Use HTML and CSS for the surrounding UI: controls, layout, labels, and status.
- Implement engine calculations in C++.
- Expose the C++ engine through a local HTTP service.
- Use CMake for the C++ build.
- Use `cpp-httplib` for the initial HTTP server unless the project owner asks for
  a different server library.
- Avoid frontend frameworks, databases, auth, remote deployment assumptions, and
  persistence until explicitly requested.

## Local Hosting And Docker

- The app should be locally hostable with Docker.
- Use a single container for the initial version.
- The C++ HTTP service should serve both static frontend assets and `/api/*`
  endpoints.
- Keep the Docker setup simple enough that a local user can build and run the
  simulator without extra infrastructure.

## Engine Boundary

- Keep simulation state browser-owned for v1.
- The browser sends current particle state and simulation parameters to the
  backend.
- The backend computes the next frame and returns updated particle state.
- Do not introduce backend-owned sessions, run IDs, persistent timelines, or
  server-side simulation storage in the initial version.
- Keep deterministic engine logic separate from HTTP request handling.

## API Direction

- Use `/api/step` as the primary engine endpoint.
- The request body should contain:
  - the current particles,
  - the timestep,
  - simulation parameters.
- The response body should contain:
  - updated particles,
  - warnings or errors needed by the UI.
- Keep the v1 API stateless and JSON-based.

## Suggested Layout

- Keep static frontend files in a dedicated frontend/static asset directory.
- Keep C++ engine and server code under `src/`.
- Keep the root CMake configuration at the project root.
- Prefer direct, readable modules over broad abstractions.

## Agent Policy

- Do not write tests unless the project owner explicitly asks.
- Do not run tests, build checks, formatters, or verification commands unless the
  project owner explicitly asks.
- If a change cannot be completed confidently without verification, explain that
  clearly and leave validation to the project owner.
- Do not add generated artifacts, dependency lock churn, or unrelated project
  structure changes without a concrete reason.
