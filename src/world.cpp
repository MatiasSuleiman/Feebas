#include "world.hpp"

#include <memory>

World::World() {
  for (int y = 0; y < 600; ++y) {
    for (int x = 0; x < 800; ++x) {
      particles.insert_or_assign({x, y}, std::make_unique<VoidParticle>(this));
    }
  }
}

void World::Create_dirt_particle_at(int x, int y) {
  particles.insert_or_assign({x, y}, std::make_unique<DirtParticle>(this));
}

bool World::there_is_dirt_particle_at(int x, int y) const {
  const Particle& particle = *particles.find({x, y})->second;
  return particle.isDirt();
}

bool World::there_is_void_particle_at(int x, int y) const {
  const Particle& particle = *particles.find({x, y})->second;
  return particle.isVoid();
}

void World::make_particle_fall(Particle* particle) {
       
        ParticleIterator iterator = this->iterator_of(particle);
        if (iterator == particles.end()) {
                return;
        }

        const Coordinate old_coordinate = iterator->first;

        if(old_coordinate.second > 0){
                Particle* particle_underneath = this->look_for_particle_underneath(iterator->first);
                particle_underneath->particle_is_falling_onto(iterator->second.get());
        }

        return;
}

void World::step() {
  for (int y = 0; y < 600; ++y) {
    for (int x = 0; x < 800; ++x) {
      Particle& particle = *particles.find({x, y})->second;
      particle.step();
    }
  }
}

World::ParticleIterator World::iterator_of(Particle* particle){
        for (ParticleIterator iterator = particles.begin(); iterator != particles.end(); ++iterator) {
                if (iterator->second.get() == particle) {
                        return iterator;
                }
        }
        return particles.end();
}

Particle* World::look_for_particle_underneath(Coordinate particle_coordinates){
        Coordinate under_coordinates{
                particle_coordinates.first,
                particle_coordinates.second - 1
        };
        ParticleIterator target_iterator = particles.find(under_coordinates);
        Particle* under_particle = target_iterator->second.get();
        return under_particle;
}

Particle* World::look_for_particle_to_the_left(Coordinate particle_coordinates){
        Coordinate left_coordinates{
                particle_coordinates.first - 1,
                particle_coordinates.second
        };

        ParticleIterator target_iterator = particles.find(left_coordinates);
        Particle* left_particle = target_iterator->second.get();
        return left_particle;
}


void World::dirt_particle_falling_onto_void(DirtParticle* dirt_particle){
        ParticleIterator particle_iterator = this->iterator_of(dirt_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;

        Coordinate new_coordinate = particle_coordinates;
        --new_coordinate.second;

        ParticleIterator particle_underneath_iterator = particles.find(new_coordinate);
        std::swap(particle_iterator->second , particle_underneath_iterator->second);
}

void World::dirt_particle_falling_onto_dirt(DirtParticle* dirt_particle){
        ParticleIterator particle_iterator = this->iterator_of(dirt_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;

         Coordinate left_coordinates{
                particle_coordinates.first - 1,
                particle_coordinates.second
        };

        Particle* particle_to_the_side = this->look_for_particle_to_the_left(particle_coordinates);
        Particle* particle_underneath_to_the_left = this->look_for_particle_underneath(left_coordinates);

        if(particle_to_the_side->isVoid() && particle_underneath_to_the_left->isVoid()){

                ParticleIterator bottom_left_edge_iterator = this->iterator_of(particle_underneath_to_the_left);
                std::swap(particle_iterator->second , bottom_left_edge_iterator->second);
        }
}
