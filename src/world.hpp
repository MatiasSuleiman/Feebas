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
        void Create_grass_particle_at(int x, int y);
        bool there_is_grass_particle_at(int x, int y) const;
        bool there_is_void_particle_at(int x, int y) const;
        void make_particle_fall(Particle* particle);
        void make_particle_fall_to_the_left(Particle* particle, Particle* bottom_left_edge_particle);
        void make_particle_fall_to_the_right(Particle* particle, Particle* bottom_right_edge_particle);
        void step();
        void dirt_particle_falling_onto_void(DirtParticle* dirt_particle);
        void dirt_particle_falling_onto_dirt(DirtParticle* dirt_particle);
        void dirt_falling_to_the_left_onto_void(DirtParticle* dirt_particle, VoidParticle* void_particle);
        void dirt_falling_to_the_right_onto_void(DirtParticle* dirt_particle, VoidParticle* void_particle);
        void grass_particle_falling_onto_void(GrassParticle* grass_particle);
        void grass_trying_to_spread();
        void grass_trying_to_spread(GrassParticle* grass_particle);
        void grass_spreads_onto(DirtParticle* dirt_particle);
  

 private:
        using Coordinate = std::pair<int, int>;
        using ParticleIterator = std::map<Coordinate, std::unique_ptr<Particle>>::iterator;

        std::map<Coordinate, std::unique_ptr<Particle>> particles;
        Particle* look_for_particle_underneath(Coordinate particle_coordinates);
        Particle* look_for_particle_to_the_left(Coordinate particle_coordinates);
        Particle* look_for_particle_to_the_right(Coordinate particle_coordinates);
        ParticleIterator iterator_of(Particle* particle);
};
