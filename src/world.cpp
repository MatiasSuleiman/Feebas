#include "world.hpp"

#include <memory>

World::World() {
  for (int y = 0; y < 600; ++y) {
    for (int x = 0; x < 800; ++x) {
      particles.insert_or_assign({x, y}, std::make_unique<VoidParticle>());
    }
  }
}

void World::Create_dirt_particle_at(int x, int y) {
  particles.insert_or_assign({x, y}, std::make_unique<DirtParticle>());
}

bool World::there_is_dirt_particle_at(int x, int y) const {
  const Particle& particle = *particles.find({x, y})->second;
  return particle.isDirt();
}

bool World::there_is_void_particle_at(int x, int y) const {
  const Particle& particle = *particles.find({x, y})->second;
  return particle.isVoid();
}

void World::step(){
        for (int y = 0; y < 600; ++y) {
                for (int x = 0; x < 800; ++x) {
                        const Particle& particle = *particles.find({x, y})->second;
                        particle.step();
                }
        }
}
