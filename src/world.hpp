#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>

#include "particles.hpp"

class World {
 public:

        World();
        int width() const;
        int height() const;
        void accept(WorldVisitor& visitor) const;
        std::string to_json() const;
        void Create_dirt_particle_at(int x, int y);
        bool there_is_dirt_particle_at(int x, int y) const;
        void Create_grass_particle_at(int x, int y);
        bool there_is_grass_particle_at(int x, int y) const;
        bool there_is_void_particle_at(int x, int y) const;
        void make_particle_fall(Particle* particle);
        void make_particle_fall_to_the_left(Particle* particle, Particle* bottom_left_edge_particle);
        void make_particle_fall_to_the_right(Particle* particle, Particle* bottom_right_edge_particle);
        void make_water_particle_fall_to_the_left(WaterParticle* water_particle, Particle* bottom_left_edge_particle);
        void make_water_particle_fall_to_the_right(WaterParticle* water_particle, Particle* bottom_right_edge_particle);
        void step();
        void dirt_particle_falling_onto_void(DirtParticle* dirt_particle);
        void dirt_particle_falling_onto_dirt(DirtParticle* dirt_particle);
        void solid_falling_onto_water(Particle* particle, WaterParticle* water_particle);
        void dirt_particle_falling_onto_water(DirtParticle* dirt_particle, WaterParticle* water_particle);
        void dirt_falling_to_the_left_onto_void(DirtParticle* dirt_particle, VoidParticle* void_particle);
        void dirt_falling_to_the_right_onto_void(DirtParticle* dirt_particle, VoidParticle* void_particle);
        void mud_particle_falling_onto_void(MudParticle* mud_particle);
        void mud_particle_falling_onto_blocking_particle(MudParticle* mud_particle);
        void mud_particle_falling_onto_water(MudParticle* mud_particle, WaterParticle* water_particle);
        void mud_falling_to_the_left_onto_void(MudParticle* mud_particle, VoidParticle* void_particle);
        void mud_falling_to_the_right_onto_void(MudParticle* mud_particle, VoidParticle* void_particle);
        void grass_particle_falling_onto_void(GrassParticle* grass_particle);
        void grass_particle_falling_onto_water(GrassParticle* grass_particle, WaterParticle* water_particle);
        void grass_trying_to_spread();
        void grass_trying_to_spread(GrassParticle* grass_particle);
        void grass_spreads_onto(DirtParticle* dirt_particle);
        void Create_water_particle_at(int x, int y);
        bool there_is_water_particle_at(int x, int y) const;
        void Create_mud_particle_at(int x, int y);
        bool there_is_mud_particle_at(int x, int y) const;
        void water_particle_falling_onto_void(WaterParticle* water_particle);
        void water_particle_falling_onto_dirt(WaterParticle* water_particle, DirtParticle* dirt_particle);
        void water_particle_falling_onto_blocking_particle(WaterParticle* water_particle);
        void water_particle_falling_onto_water(WaterParticle* falling_water_particle, WaterParticle* blocking_water_particle);
        bool can_be_moved_to_the_left(Particle* particle);
        bool can_be_moved_to_the_right(Particle* particle);
        int amount_of_water_to_the_left_of(Particle* particle);
        int amount_of_water_to_the_right_of(Particle* particle);
        bool water_can_be_pushed_upwards(WaterParticle* water_particle);
        void water_falling_to_the_left_onto_void(WaterParticle* water_particle, VoidParticle* void_particle);
        void water_falling_to_the_right_onto_void(WaterParticle* water_particle, VoidParticle* void_particle);
  

 private:
        using Coordinate = std::pair<int, int>;
        using ParticleIterator = std::map<Coordinate, std::unique_ptr<Particle>>::iterator;
        using ConstParticleIterator = std::map<Coordinate, std::unique_ptr<Particle>>::const_iterator;

        std::map<Coordinate, std::unique_ptr<Particle>> particles;
        VoidParticle void_particle;
        bool coordinate_is_inside_world(Coordinate coordinate) const;
        Particle* particle_at(Coordinate coordinate);
        const Particle* particle_at(Coordinate coordinate) const;
        Particle* look_for_particle_to_the_left_of(Particle* particle);
        Particle* look_for_particle_to_the_right_of(Particle* particle);
        Particle* look_for_particle_above(Particle* particle);
        Particle* look_for_particle_underneath(Particle* particle);
        Particle* look_for_particle_underneath(Coordinate particle_coordinates);
        void move_particle_to(ParticleIterator particle_iterator, Coordinate new_coordinate);
        void move_water_chain_to_the_left(WaterParticle* water_particle);
        void move_water_chain_to_the_right(WaterParticle* water_particle);
        void move_water_chain_upwards(WaterParticle* water_particle);
        bool water_chain_can_overflow_to_the_left(Particle* particle);
        bool water_chain_can_overflow_to_the_right(Particle* particle);
        void move_water_chain_overflow_to_the_left(WaterParticle* water_particle);
        void move_water_chain_overflow_to_the_right(WaterParticle* water_particle);
        ParticleIterator iterator_of(Particle* particle);
};
