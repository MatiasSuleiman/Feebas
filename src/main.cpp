#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include <httplib.h>

#include "world.hpp"

namespace {

using Coordinate = std::pair<int, int>;

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

bool parse_int_value(const std::string& raw_value, int& value) {
  std::size_t consumed_characters = 0;

  try {
    value = std::stoi(raw_value, &consumed_characters);
  } catch (...) {
    return false;
  }

  return consumed_characters == raw_value.size();
}

bool read_cells_param(const httplib::Request& req, std::vector<Coordinate>& cells) {
  if (!req.has_param("cells")) {
    return false;
  }

  const std::string raw_cells = req.get_param_value("cells");
  if (raw_cells.empty()) {
    return false;
  }

  std::size_t cell_start = 0;
  while (cell_start <= raw_cells.size()) {
    const std::size_t cell_end = raw_cells.find(',', cell_start);
    const std::size_t token_end = cell_end == std::string::npos ? raw_cells.size() : cell_end;
    const std::size_t separator = raw_cells.find(':', cell_start);

    if (cell_start == token_end || separator == std::string::npos || separator >= token_end) {
      return false;
    }

    int x = 0;
    int y = 0;
    if (!parse_int_value(raw_cells.substr(cell_start, separator - cell_start), x) ||
        !parse_int_value(raw_cells.substr(separator + 1, token_end - separator - 1), y)) {
      return false;
    }

    cells.push_back({x, y});

    if (cell_end == std::string::npos) {
      break;
    }

    cell_start = cell_end + 1;
  }

  return !cells.empty();
}

bool coordinates_are_inside_world(const World& world, int x, int y) {
  return x >= 0 && y >= 0 && x < world.width() && y < world.height();
}

bool cells_are_inside_world(const World& world, const std::vector<Coordinate>& cells) {
  for (const Coordinate& cell : cells) {
    if (!coordinates_are_inside_world(world, cell.first, cell.second)) {
      return false;
    }
  }

  return true;
}

void set_bad_request(httplib::Response& res, const std::string& message) {
  res.status = 400;
  res.set_content("{\"error\":\"" + message + "\"}", "application/json");
}

template <typename CreateParticle>
void create_particles_from_cells(
    const httplib::Request& req,
    httplib::Response& res,
    World& world,
    std::mutex& world_mutex,
    CreateParticle create_particle) {
  std::vector<Coordinate> cells;
  if (!read_cells_param(req, cells)) {
    set_bad_request(res, "cells query parameter must contain x:y coordinate pairs");
    return;
  }

  std::lock_guard<std::mutex> lock(world_mutex);
  if (!cells_are_inside_world(world, cells)) {
    set_bad_request(res, "coordinates must be inside the world");
    return;
  }

  for (const Coordinate& cell : cells) {
    create_particle(world, cell.first, cell.second);
  }

  res.set_content(world.consume_changes_json(), "application/json");
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
    res.set_content(
      "{\"width\":" + std::to_string(world.width()) +
        ",\"height\":" + std::to_string(world.height()) + "}",
      "application/json"
    );
  });

  server.Post("/world/step", [&world, &world_mutex](const httplib::Request&, httplib::Response& res) {
    std::lock_guard<std::mutex> lock(world_mutex);
    world.step();
    res.set_content(world.consume_changes_json(), "application/json");
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
    res.set_content(world.consume_changes_json(), "application/json");
  });

  server.Post("/world/create-dirt-particles", [&world, &world_mutex](const httplib::Request& req, httplib::Response& res) {
    create_particles_from_cells(req, res, world, world_mutex, [](World& target_world, int x, int y) {
      target_world.Create_dirt_particle_at(x, y);
    });
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
    res.set_content(world.consume_changes_json(), "application/json");
  });

  server.Post("/world/create-grass-particles", [&world, &world_mutex](const httplib::Request& req, httplib::Response& res) {
    create_particles_from_cells(req, res, world, world_mutex, [](World& target_world, int x, int y) {
      target_world.Create_grass_particle_at(x, y);
    });
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
    res.set_content(world.consume_changes_json(), "application/json");
  });

  server.Post("/world/create-water-particles", [&world, &world_mutex](const httplib::Request& req, httplib::Response& res) {
    create_particles_from_cells(req, res, world, world_mutex, [](World& target_world, int x, int y) {
      target_world.Create_water_particle_at(x, y);
    });
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
    res.set_content(world.consume_changes_json(), "application/json");
  });

  server.Post("/world/create-mud-particles", [&world, &world_mutex](const httplib::Request& req, httplib::Response& res) {
    create_particles_from_cells(req, res, world, world_mutex, [](World& target_world, int x, int y) {
      target_world.Create_mud_particle_at(x, y);
    });
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
    res.set_content(world.consume_changes_json(), "application/json");
  });

  server.Post("/world/create-stone-particles", [&world, &world_mutex](const httplib::Request& req, httplib::Response& res) {
    create_particles_from_cells(req, res, world, world_mutex, [](World& target_world, int x, int y) {
      target_world.Create_stone_particle_at(x, y);
    });
  });

  server.Post("/world/create-wood-particle-at", [&world, &world_mutex](const httplib::Request& req, httplib::Response& res) {
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

    world.Create_wood_particle_at(x, y);
    res.set_content(world.consume_changes_json(), "application/json");
  });

  server.Post("/world/create-wood-particles", [&world, &world_mutex](const httplib::Request& req, httplib::Response& res) {
    create_particles_from_cells(req, res, world, world_mutex, [](World& target_world, int x, int y) {
      target_world.Create_wood_particle_at(x, y);
    });
  });

  server.Post("/world/create-fire-particle-at", [&world, &world_mutex](const httplib::Request& req, httplib::Response& res) {
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

    world.Create_fire_particle_at(x, y);
    res.set_content(world.consume_changes_json(), "application/json");
  });

  server.Post("/world/create-fire-particles", [&world, &world_mutex](const httplib::Request& req, httplib::Response& res) {
    create_particles_from_cells(req, res, world, world_mutex, [](World& target_world, int x, int y) {
      target_world.Create_fire_particle_at(x, y);
    });
  });

  server.Post("/world/create-TNT-particle-at", [&world, &world_mutex](const httplib::Request& req, httplib::Response& res) {
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

    world.Create_TNT_particle_at(x, y);
    res.set_content(world.consume_changes_json(), "application/json");
  });

  server.Post("/world/create-TNT-particles", [&world, &world_mutex](const httplib::Request& req, httplib::Response& res) {
    create_particles_from_cells(req, res, world, world_mutex, [](World& target_world, int x, int y) {
      target_world.Create_TNT_particle_at(x, y);
    });
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
