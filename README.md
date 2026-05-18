# Feebas Particle Simulator

A tiny local particle simulator webapp scaffold. The current milestone serves a
blank canvas page from a C++ HTTP server inside Docker.

## Run Locally With Docker

Build the image:

```sh
docker build -t feebas-sim .
```

Run the server:

```sh
docker run --rm -p 8080:8080 feebas-sim
```

Expected startup output:

```text
Feebas simulator is running.
Open http://localhost:8080 in your browser.
Container bind address: http://0.0.0.0:8080
Serving static files from /app/frontend
```

Open `http://localhost:8080` in a browser.

The health endpoint is available at `http://localhost:8080/api/health`.

## Run C++ Tests

For a local test-only build, disable the HTTP server target so CMake does not
need `cpp-httplib`:

```sh
cmake -S . -B build-tests -DFEEBAS_BUILD_SERVER=OFF
cmake --build build-tests
ctest --test-dir build-tests
```
