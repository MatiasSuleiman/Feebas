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

void World::Create_grass_particle_at(int x, int y) {
  particles.insert_or_assign({x, y}, std::make_unique<GrassParticle>(this));
}

bool World::there_is_dirt_particle_at(int x, int y) const {
  const Particle& particle = *particles.find({x, y})->second;
  return particle.isDirt();
}

bool World::there_is_grass_particle_at(int x, int y) const {
  const Particle& particle = *particles.find({x, y})->second;
  return particle.isGrass();
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

void World::make_particle_fall_to_the_left(Particle* particle, Particle* bottom_left_edge_particle) {
        bottom_left_edge_particle->dirt_falling_to_the_left(static_cast<DirtParticle*>(particle));
}

void World::make_particle_fall_to_the_right(Particle* particle, Particle* bottom_right_edge_particle) {
        bottom_right_edge_particle->dirt_falling_to_the_right(static_cast<DirtParticle*>(particle));
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

Particle* World::look_for_particle_to_the_right(Coordinate particle_coordinates){
        Coordinate right_coordinates{
                particle_coordinates.first + 1,
                particle_coordinates.second
        };

        ParticleIterator target_iterator = particles.find(right_coordinates);
        Particle* right_particle = target_iterator->second.get();
        return right_particle;
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

        if (particle_coordinates.first != 0) {
                Coordinate left_coordinates{
                        particle_coordinates.first - 1,
                        particle_coordinates.second
                };

                Particle* particle_to_the_left = this->look_for_particle_to_the_left(particle_coordinates);
                Particle* particle_underneath_to_the_left = this->look_for_particle_underneath(left_coordinates);

                if(particle_to_the_left->dirt_can_fall_through() && particle_underneath_to_the_left->dirt_can_fall_through()){
                        this->make_particle_fall_to_the_left(dirt_particle, particle_underneath_to_the_left);
                        return;
                }
        }

        Coordinate right_coordinates{
                particle_coordinates.first + 1,
                particle_coordinates.second
        };
        Particle* particle_to_the_right = this->look_for_particle_to_the_right(particle_coordinates);
        Particle* particle_underneath_to_the_right = this->look_for_particle_underneath(right_coordinates);

        if(particle_to_the_right->dirt_can_fall_through() && particle_underneath_to_the_right->dirt_can_fall_through()){
                this->make_particle_fall_to_the_right(dirt_particle, particle_underneath_to_the_right);
        }
}

void World::dirt_falling_to_the_left_onto_void(DirtParticle* dirt_particle, VoidParticle* void_particle){
        ParticleIterator particle_iterator = this->iterator_of(dirt_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        ParticleIterator void_iterator = this->iterator_of(void_particle);
        if (void_iterator == particles.end()) {
                return;
        }

        std::swap(particle_iterator->second , void_iterator->second);
}

void World::dirt_falling_to_the_right_onto_void(DirtParticle* dirt_particle, VoidParticle* void_particle){
        ParticleIterator particle_iterator = this->iterator_of(dirt_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        ParticleIterator void_iterator = this->iterator_of(void_particle);
        if (void_iterator == particles.end()) {
                return;
        }

        std::swap(particle_iterator->second , void_iterator->second);
}

void World::grass_particle_falling_onto_void(GrassParticle* grass_particle){

        ParticleIterator particle_iterator = this->iterator_of(grass_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;

        Coordinate new_coordinate = particle_coordinates;
        --new_coordinate.second;

        ParticleIterator particle_underneath_iterator = particles.find(new_coordinate);
        std::swap(particle_iterator->second , particle_underneath_iterator->second);
}

void World::grass_trying_to_spread(){
        for (ParticleIterator iterator = particles.begin(); iterator != particles.end(); ++iterator) {
                if (iterator->second->isGrass()) {
                        this->grass_trying_to_spread(static_cast<GrassParticle*>(iterator->second.get()));
                }
        }
}

void World::grass_trying_to_spread(GrassParticle* grass_particle){
        
        ParticleIterator particle_iterator = this->iterator_of(grass_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;

        Coordinate left_coordinate = particle_coordinates;
        --left_coordinate.first;

        Coordinate right_coordinate = particle_coordinates;
        ++right_coordinate.first;

        if(left_coordinate.first >= 0){

                ParticleIterator particle_to_the_left_iterator = particles.find(left_coordinate);
                particle_to_the_left_iterator->second->grass_trying_to_spread_onto();
        }

        if(right_coordinate.first < 800){

                ParticleIterator particle_to_the_right_iterator = particles.find(right_coordinate);
                particle_to_the_right_iterator->second->grass_trying_to_spread_onto();
        }
}

void World::grass_spreads_onto(DirtParticle* dirt_particle){
        ParticleIterator particle_iterator = this->iterator_of(dirt_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        particle_iterator->second = std::make_unique<GrassParticle>(this);
}
