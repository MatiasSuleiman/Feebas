#include "world.hpp"

World::World() {
  DirtParticle particle;
  particles.push_back({particle, {0, 0}});
}
