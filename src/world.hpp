#pragma once

#include <tuple>
#include <vector>

#include "particles.hpp"

class World {
 public:
  World();

 private:
  std::vector<std::tuple<DirtParticle, std::tuple<int, int>>> particles;
};
