FROM debian:bookworm-slim AS build

RUN apt-get update \
  && apt-get install -y --no-install-recommends \
    build-essential \
    catch2 \
    cmake \
    libcpp-httplib-dev \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY CMakeLists.txt ./
COPY src ./src
COPY tests ./tests

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
  && cmake --build build --config Release

FROM debian:bookworm-slim

RUN apt-get update \
  && apt-get install -y --no-install-recommends \
    ca-certificates \
    libcpp-httplib-dev \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=build /app/build/feebas-server ./feebas-server
COPY frontend ./frontend

ENV PORT=8080
ENV STATIC_DIR=/app/frontend

EXPOSE 8080

CMD ["./feebas-server"]
