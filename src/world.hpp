#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "particles.hpp"
#include "world_cell_change.hpp"

class World {
 public:

        World();
        int width() const;
        int height() const;
        void accept(WorldVisitor& visitor) const;
        std::string consume_changes_json();
        void Create_dirt_particle_at(int x, int y);
        bool there_is_dirt_particle_at(int x, int y) const;
        void Create_grass_particle_at(int x, int y);
        bool there_is_grass_particle_at(int x, int y) const;
        bool there_is_void_particle_at(int x, int y) const;
        void make_particle_fall_to_the_left(Particle* particle, Particle* bottom_left_edge_particle);
        void make_particle_fall_to_the_right(Particle* particle, Particle* bottom_right_edge_particle);
        void make_water_particle_fall_to_the_left(WaterParticle* water_particle, Particle* bottom_left_edge_particle);
        void make_water_particle_fall_to_the_right(WaterParticle* water_particle, Particle* bottom_right_edge_particle);
        void step();
        void particle_pushing_onto_void(Particle* particle, VoidParticle* void_particle);
        void dirt_particle_pushing_onto_dirt(DirtParticle* pushing_dirt, DirtParticle* blocking_dirt);
        void solid_pushing_onto_water(Particle* particle, WaterParticle* water_particle);
        void wood_pushing_onto_water(WoodParticle* wood_particle, WaterParticle* water_particle);
        void dirt_falling_to_the_left_onto_void(DirtParticle* dirt_particle, VoidParticle* void_particle);
        void dirt_falling_to_the_right_onto_void(DirtParticle* dirt_particle, VoidParticle* void_particle);
        void dirt_falling_to_the_left_onto_fire(DirtParticle* dirt_particle, FireParticle* fire_particle);
        void dirt_falling_to_the_right_onto_fire(DirtParticle* dirt_particle, FireParticle* fire_particle);
        void mud_particle_pushing_onto_void(MudParticle* mud_particle);
        void mud_particle_pushing_onto_mud(MudParticle* pushing_mud, MudParticle* blocking_mud);
        void mud_particle_pushing_onto_water(MudParticle* mud_particle, WaterParticle* water_particle);
        void stone_particle_pushing_onto_void(StoneParticle* stone_particle);
        void stone_particle_pushing_onto_water(StoneParticle* stone_particle, WaterParticle* water_particle);
        void wood_particle_pushing_onto_void(WoodParticle* wood_particle);
        void fire_particle_pushing_onto_void(FireParticle* fire_particle);
        void mud_falling_to_the_left_onto_void(MudParticle* mud_particle, VoidParticle* void_particle);
        void mud_falling_to_the_right_onto_void(MudParticle* mud_particle, VoidParticle* void_particle);
        void mud_falling_to_the_left_onto_fire(MudParticle* mud_particle, FireParticle* fire_particle);
        void mud_falling_to_the_right_onto_fire(MudParticle* mud_particle, FireParticle* fire_particle);
        void grass_particle_pushing_onto_void(GrassParticle* grass_particle);
        void grass_particle_pushing_onto_water(GrassParticle* grass_particle, WaterParticle* water_particle);
        void grass_trying_to_spread();
        void grass_trying_to_spread(GrassParticle* grass_particle);
        void grass_spreads_onto(DirtParticle* dirt_particle);
        void fire_trying_to_spread(FireParticle* fire_particle);
        void fire_spreading_onto(Particle* particle);
        void Create_water_particle_at(int x, int y);
        bool there_is_water_particle_at(int x, int y) const;
        void Create_mud_particle_at(int x, int y);
        bool there_is_mud_particle_at(int x, int y) const;
        void Create_stone_particle_at(int x, int y);
        bool there_is_stone_particle_at(int x, int y) const;
        void Create_wood_particle_at(int x, int y);
        bool there_is_wood_particle_at(int x, int y) const;
        void Create_fire_particle_at(int x, int y);
        bool there_is_fire_particle_at(int x, int y) const;

        void water_particle_pushing_onto_void(WaterParticle* water_particle);
        void water_particle_pushing_onto_dirt(WaterParticle* water_particle, DirtParticle* dirt_particle);
        void water_particle_pushing_onto_blocking_particle(WaterParticle* water_particle);

        bool can_be_moved_to_the_left(Particle* particle);
        bool can_be_moved_to_the_right(Particle* particle);

        int amount_of_water_to_the_left_of(Particle* particle);
        int amount_of_water_to_the_right_of(Particle* particle);
        bool water_can_be_pushed_upwards(WaterParticle* water_particle);
        bool isDamStructureUpwards(Particle* particle);
        bool isDamStructureDownwards(Particle* particle);
        int distance_to_overflow_to_the_left_from(WaterParticle* water_particle);
        int distance_to_overflow_to_the_right_from(WaterParticle* water_particle);

        void water_falling_to_the_left_onto_void(WaterParticle* water_particle, VoidParticle* void_particle);
        void water_falling_to_the_right_onto_void(WaterParticle* water_particle, VoidParticle* void_particle);

        void fire_died(FireParticle* fire_particle);
        void reset_speed(Particle* particle);
        void support_stone(StoneParticle* stone_particle);
        void particle_clash(Particle* clashing_particle, Particle* blocking_particle);
        void water_pushing_onto_water(WaterParticle* pushing_water, WaterParticle* pushed_water);
        void water_pushing_onto_solid(WaterParticle* water, Particle* solid);
        void water_pushing_onto_wood(WaterParticle* water_particle, WoodParticle* wood_particle);
        void water_pushing_onto_fire(WaterParticle* water_particle, FireParticle* fire_particle);
        void fire_pushing_onto_water(FireParticle* fire_particle, WaterParticle* water_particle);
        void crash_onto_wall(Particle* particle, int attempted_x, int attempted_y);
        void turn_dirt_into_mud(DirtParticle* dirt_particle);
        bool water_can_overflow(WaterParticle* water_particle);

 private:
        using Coordinate = std::pair<int, int>;
        struct Vector {
                int x;
                int y;
        };
        struct CoordinateHash {
                
                std::size_t operator()(const Coordinate& coordinate) const {
                        const std::size_t first_hash = std::hash<int>{}(coordinate.first);
                        const std::size_t second_hash = std::hash<int>{}(coordinate.second);
                        return first_hash ^ (second_hash + 0x9e3779b9 + (first_hash << 6) + (first_hash >> 2));
                }
        };
        struct ParticleState {
                std::unique_ptr<Particle> particle;
                Vector speed;
                Vector acceleration;
        };
        using ParticleStorage = std::unordered_map<Coordinate, ParticleState, CoordinateHash>;
        using ParticleIterator = ParticleStorage::iterator;
        using ConstParticleIterator = ParticleStorage::const_iterator;

        ParticleStorage particles;
        std::unordered_map<const Particle*, Coordinate> particle_coordinates_by_pointer;
        VoidParticle void_particle;
        int current_revision = 0;
        std::vector<WorldCellChange> changes;
        std::vector<std::unique_ptr<Particle>> retired_particles;
        bool coordinate_is_inside_world(Coordinate coordinate) const;
        Particle* particle_at(Coordinate coordinate);
        const Particle* particle_at(Coordinate coordinate) const;
        void record_change_at(Coordinate coordinate, const Particle* particle);
        Particle* look_for_particle_to_the_left_of(Particle* particle);
        Particle* look_for_particle_to_the_right_of(Particle* particle);
        Particle* look_for_particle_above(Particle* particle);
        Particle* look_for_particle_underneath(Particle* particle);
        Particle* look_for_particle_underneath(Coordinate particle_coordinates);
        void index_particle_at(Coordinate coordinate, const Particle* particle);
        void unindex_particle(const Particle* particle);
        void retire_particle(ParticleIterator particle_iterator);
        void replace_particle_at(Coordinate coordinate, std::unique_ptr<Particle> particle);
        void erase_particle(ParticleIterator particle_iterator);
        void move_particle_to(ParticleIterator particle_iterator, Coordinate new_coordinate);
        void applyForce(Particle* particle, Vector force);
        void apply_acceleration_to_speed(Particle* particle);
        void reset_acceleration(Particle* particle);
        void reset_vertical_speed(Particle* particle);
        void reduce_vertical_speed(Particle* particle);
        void apply_friction_to(Particle* particle);
        void apply_movement_to(Particle* particle);
        int maximum_distance_it_can_travel_to_the_left_capped_at(Particle* particle, int distance);
        Coordinate maximum_uninterrupted_movement_to(Particle* particle, Vector movement);
        int maximum_distance_it_can_travel_to_the_right_capped_at(Particle* particle, int distance);
        void move_water_chain_to_the_left(Particle* particle);
        void move_water_chain_to_the_right(Particle* particle);
        void move_water_chain_upwards(WaterParticle* water_particle);
        bool floatable_column_can_move_upwards(Particle* particle);
        bool move_floatable_column_upwards(Particle* particle);
        WaterParticle* left_water_edge(WaterParticle* water_particle);
        WaterParticle* right_water_edge(WaterParticle* water_particle);
        void apply_horizontal_water_wave_force(WaterParticle* pushed_water, Vector force);
        bool water_can_overflow_to_the_left_from(WaterParticle* water_particle);
        bool water_can_overflow_to_the_right_from(WaterParticle* water_particle);
        bool overflow_target_above(WaterParticle* water_particle, Coordinate& target_coordinate);
        bool overflow_target_to_the_left_from(WaterParticle* water_particle, Coordinate& target_coordinate);
        bool overflow_target_to_the_right_from(WaterParticle* water_particle, Coordinate& target_coordinate);
        bool make_water_overflow(WaterParticle* clashed_water, Vector impact);
        ParticleIterator iterator_of(Particle* particle);
        std::vector<Coordinate> coordinates_vector_passes_through_starting_at(Coordinate starting_point, Vector trayectory);
        std::vector<Coordinate> coordinates_wide_vector_passes_through_starting_at(Coordinate starting_point, Vector trayectory);
        std::vector<Coordinate> coordinates_tall_vector_passes_through_starting_at(Coordinate starting_point, Vector trayectory);
};
