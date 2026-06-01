#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <mutex>
#include <string>

#include <httplib.h>

#include "world.hpp"

namespace {

int read_port() {
  const char* value = std::getenv("PORT");
  if (value == nullptr || std::string(value).empty()) {
    return 8080;
  }

  try {
    return std::stoi(value);
  } catch (...) {
    return 8080;
  }
}

std::string read_static_dir() {
  const char* value = std::getenv("STATIC_DIR");
  if (value == nullptr || std::string(value).empty()) {
    return "frontend";
  }

  return value;
}

bool read_int_param(const httplib::Request& req, const std::string& name, int& value) {
  if (!req.has_param(name)) {
    return false;
  }

  const std::string raw_value = req.get_param_value(name);
  std::size_t consumed_characters = 0;

  try {
    value = std::stoi(raw_value, &consumed_characters);
  } catch (...) {
    return false;
  }

  return consumed_characters == raw_value.size();
}

bool coordinates_are_inside_world(const World& world, int x, int y) {
  return x >= 0 && y >= 0 && x < world.width() && y < world.height();
}

void set_bad_request(httplib::Response& res, const std::string& message) {
  res.status = 400;
  res.set_content("{\"error\":\"" + message + "\"}", "application/json");
}

}  // namespace

int main() {
  const int port = read_port();
  const std::string static_dir = read_static_dir();
  World world;
  std::mutex world_mutex;

  httplib::Server server;

  server.Get("/world", [&world, &world_mutex](const httplib::Request&, httplib::Response& res) {
    std::lock_guard<std::mutex> lock(world_mutex);
    res.set_content(world.to_json(), "application/json");
  });

  server.Post("/world/step", [&world, &world_mutex](const httplib::Request&, httplib::Response& res) {
    std::lock_guard<std::mutex> lock(world_mutex);
    world.step();
    res.set_content(world.to_json(), "application/json");
  });

  server.Post("/world/create-dirt-particle-at", [&world, &world_mutex](const httplib::Request& req, httplib::Response& res) {
    int x = 0;
    int y = 0;
    if (!read_int_param(req, "x", x) || !read_int_param(req, "y", y)) {
      set_bad_request(res, "x and y query parameters must be integers");
      return;
    }

    std::lock_guard<std::mutex> lock(world_mutex);
    if (!coordinates_are_inside_world(world, x, y)) {
      set_bad_request(res, "coordinates must be inside the world");
      return;
    }

    world.Create_dirt_particle_at(x, y);
    res.set_content(world.to_json(), "application/json");
  });

  server.Post("/world/create-grass-particle-at", [&world, &world_mutex](const httplib::Request& req, httplib::Response& res) {
    int x = 0;
    int y = 0;
    if (!read_int_param(req, "x", x) || !read_int_param(req, "y", y)) {
      set_bad_request(res, "x and y query parameters must be integers");
      return;
    }

    std::lock_guard<std::mutex> lock(world_mutex);
    if (!coordinates_are_inside_world(world, x, y)) {
      set_bad_request(res, "coordinates must be inside the world");
      return;
    }

    world.Create_grass_particle_at(x, y);
    res.set_content(world.to_json(), "application/json");
  });

  server.Post("/world/create-water-particle-at", [&world, &world_mutex](const httplib::Request& req, httplib::Response& res) {
    int x = 0;
    int y = 0;
    if (!read_int_param(req, "x", x) || !read_int_param(req, "y", y)) {
      set_bad_request(res, "x and y query parameters must be integers");
      return;
    }

    std::lock_guard<std::mutex> lock(world_mutex);
    if (!coordinates_are_inside_world(world, x, y)) {
      set_bad_request(res, "coordinates must be inside the world");
      return;
    }

    world.Create_water_particle_at(x, y);
    res.set_content(world.to_json(), "application/json");
  });

  server.Post("/world/create-mud-particle-at", [&world, &world_mutex](const httplib::Request& req, httplib::Response& res) {
    int x = 0;
    int y = 0;
    if (!read_int_param(req, "x", x) || !read_int_param(req, "y", y)) {
      set_bad_request(res, "x and y query parameters must be integers");
      return;
    }

    std::lock_guard<std::mutex> lock(world_mutex);
    if (!coordinates_are_inside_world(world, x, y)) {
      set_bad_request(res, "coordinates must be inside the world");
      return;
    }

    world.Create_mud_particle_at(x, y);
    res.set_content(world.to_json(), "application/json");
  });

  server.Post("/world/create-stone-particle-at", [&world, &world_mutex](const httplib::Request& req, httplib::Response& res) {
    int x = 0;
    int y = 0;
    if (!read_int_param(req, "x", x) || !read_int_param(req, "y", y)) {
      set_bad_request(res, "x and y query parameters must be integers");
      return;
    }

    std::lock_guard<std::mutex> lock(world_mutex);
    if (!coordinates_are_inside_world(world, x, y)) {
      set_bad_request(res, "coordinates must be inside the world");
      return;
    }

    world.Create_stone_particle_at(x, y);
    res.set_content(world.to_json(), "application/json");
  });

  if (!server.set_mount_point("/", static_dir)) {
    std::cerr << "Failed to mount static directory: " << static_dir << '\n';
    return 1;
  }

  std::cout << "Feebas simulator is running." << std::endl;
  std::cout << "Open http://localhost:" << port << " in your browser." << std::endl;
  std::cout << "Container bind address: http://0.0.0.0:" << port << std::endl;
  std::cout << "Serving static files from " << static_dir << std::endl;

  if (!server.listen("0.0.0.0", port)) {
    std::cerr << "Failed to start server on port " << port << '\n';
    return 1;
  }

  return 0;
}
