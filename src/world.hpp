#pragma once

#include <map>
#include <memory>
#include <utility>

#include "particles.hpp"

class World {
 public:
  World();
  void Create_dirt_particle_at(int x, int y);
  bool there_is_dirt_particle_at(int x, int y) const;
  bool there_is_void_particle_at(int x, int y) const;
  void step();

 private:
  using Coordinate = std::pair<int, int>;

  std::map<Coordinate, std::unique_ptr<Particle>> particles;
};
