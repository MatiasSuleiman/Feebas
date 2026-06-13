#include "world.hpp"

#include <algorithm>
#include <cstdlib>
#include <memory>
#include <vector>

#include "world_changes_to_json_visitor.hpp"

namespace {

constexpr int world_width = 200;
constexpr int world_height = 200;

}  // namespace

World::World() : void_particle(this) {
}

int World::width() const {
  return world_width;
}

int World::height() const {
  return world_height;
}

void World::accept(WorldVisitor& visitor) const {
  visitor.visit_world(width(), height());

  for (int y = 0; y < height(); ++y) {
    for (int x = 0; x < width(); ++x) {
      const auto particle_iterator = particles.find({x, y});
      if (particle_iterator != particles.end()) {
        particle_iterator->second.particle->accept(visitor);
      } else {
        void_particle.accept(visitor);
      }
    }
  }

  visitor.finish_visiting_world();
}

std::string World::consume_changes_json() {
        ++current_revision;

        WorldChangesToJSONVisitor visitor(current_revision);
        std::string json = visitor.json_for(changes);
        changes.clear();
        retired_particles.clear();
        return json;
}

void World::Create_dirt_particle_at(int x, int y) {
  Coordinate coordinate{x, y};
  replace_particle_at(coordinate, std::make_unique<DirtParticle>(this));
}

void World::Create_grass_particle_at(int x, int y) {
  Coordinate coordinate{x, y};
  replace_particle_at(coordinate, std::make_unique<GrassParticle>(this));
}

void World::Create_water_particle_at(int x, int y) {
  Coordinate coordinate{x, y};
  replace_particle_at(coordinate, std::make_unique<WaterParticle>(this));
}

void World::Create_mud_particle_at(int x, int y) {
  Coordinate coordinate{x, y};
  replace_particle_at(coordinate, std::make_unique<MudParticle>(this));
}

void World::Create_stone_particle_at(int x, int y) {
  Coordinate coordinate{x, y};
  replace_particle_at(coordinate, std::make_unique<StoneParticle>(this));
}

void World::Create_wood_particle_at(int x, int y) {
  Coordinate coordinate{x, y};
  replace_particle_at(coordinate, std::make_unique<WoodParticle>(this));
}

void World::Create_fire_particle_at(int x, int y) {
  Coordinate coordinate{x, y};
  replace_particle_at(coordinate, std::make_unique<FireParticle>(this));
}

bool World::there_is_dirt_particle_at(int x, int y) const {
  const Particle* particle = particle_at({x, y});
  return particle != nullptr && particle->isDirt();
}

bool World::there_is_water_particle_at(int x, int y) const {
  const Particle* particle = particle_at({x, y});
  return particle != nullptr && particle->isWater();
}

bool World::there_is_mud_particle_at(int x, int y) const {
  const Particle* particle = particle_at({x, y});
  return particle != nullptr && particle->isMud();
}

bool World::there_is_stone_particle_at(int x, int y) const {
  const Particle* particle = particle_at({x, y});
  return particle != nullptr && particle->isStone();
}

bool World::there_is_grass_particle_at(int x, int y) const {
  const Particle* particle = particle_at({x, y});
  return particle != nullptr && particle->isGrass();
}

bool World::there_is_void_particle_at(int x, int y) const {
  const Particle* particle = particle_at({x, y});
  return particle != nullptr && particle->isVoid();
}

bool World::there_is_wood_particle_at(int x, int y) const {
  const Particle* particle = particle_at({x, y});
  return particle != nullptr && particle->isWood();
}

bool World::there_is_fire_particle_at(int x, int y) const {
  const Particle* particle = particle_at({x, y});
  return particle != nullptr && particle->isFire();
}

void World::make_particle_fall_to_the_left(Particle* particle, Particle* bottom_left_edge_particle) {
        bottom_left_edge_particle->dirt_pushing_to_the_left(static_cast<DirtParticle*>(particle));
}

void World::make_particle_fall_to_the_right(Particle* particle, Particle* bottom_right_edge_particle) {
        bottom_right_edge_particle->dirt_pushing_to_the_right(static_cast<DirtParticle*>(particle));
}

void World::make_water_particle_fall_to_the_left(WaterParticle* water_particle, Particle* bottom_left_edge_particle) {
        bottom_left_edge_particle->water_pushing_to_the_left(water_particle);
}

void World::make_water_particle_fall_to_the_right(WaterParticle* water_particle, Particle* bottom_right_edge_particle) {
        bottom_right_edge_particle->water_pushing_to_the_right(water_particle);
}

void World::step() {
        std::vector<Coordinate> particle_coordinates;
        particle_coordinates.reserve(particles.size());

        for (const auto& particle_entry : particles) {
                particle_coordinates.push_back(particle_entry.first);
        }

        std::sort(particle_coordinates.begin(), particle_coordinates.end(),
                [](Coordinate left, Coordinate right) {
                        if (left.second != right.second) {
                                return left.second < right.second;
                        }
                        return left.first < right.first;
                });

        for (Coordinate coordinate : particle_coordinates) {
                ParticleIterator iterator = particles.find(coordinate);
                if (iterator != particles.end()) {

                        Particle* particle = iterator->second.particle.get();
                        apply_acceleration_to_speed(particle);
                        reset_acceleration(particle);

                        iterator = iterator_of(particle);
                        if (iterator != particles.end()) {
                                iterator->second.particle->step();
                        }

                        iterator = iterator_of(particle);
                        if (iterator != particles.end()) {
                                apply_movement_to(particle);
                        }
                }
        }
}

bool World::coordinate_is_inside_world(Coordinate coordinate) const {
        return coordinate.first >= 0 && coordinate.second >= 0 &&
                coordinate.first < world_width && coordinate.second < world_height;
}

Particle* World::particle_at(Coordinate coordinate) {
        if (!coordinate_is_inside_world(coordinate)) {
                return nullptr;
        }

        ParticleIterator target_iterator = particles.find(coordinate);
        if (target_iterator == particles.end()) {
                return &void_particle;
        }

        return target_iterator->second.particle.get();
}

const Particle* World::particle_at(Coordinate coordinate) const {
        if (!coordinate_is_inside_world(coordinate)) {
                return nullptr;
        }

        ConstParticleIterator target_iterator = particles.find(coordinate);
        if (target_iterator == particles.end()) {
                return &void_particle;
        }

        return target_iterator->second.particle.get();
}

void World::record_change_at(Coordinate coordinate, const Particle* particle) {
        if (!coordinate_is_inside_world(coordinate) || particle == nullptr) {
                return;
        }

        changes.push_back({
                coordinate.first,
                coordinate.second,
                particle
        });
}

Particle* World::look_for_particle_to_the_left_of(Particle* particle) {
        ParticleIterator particle_iterator = this->iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return nullptr;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        Coordinate left_coordinates{
                particle_coordinates.first - 1,
                particle_coordinates.second
        };

        return particle_at(left_coordinates);
}

Particle* World::look_for_particle_to_the_right_of(Particle* particle) {
        ParticleIterator particle_iterator = this->iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return nullptr;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        Coordinate right_coordinates{
                particle_coordinates.first + 1,
                particle_coordinates.second
        };

        return particle_at(right_coordinates);
}

Particle* World::look_for_particle_above(Particle* particle) {
        ParticleIterator particle_iterator = this->iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return nullptr;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        Coordinate above_coordinates{
                particle_coordinates.first,
                particle_coordinates.second + 1
        };

        return particle_at(above_coordinates);
}

Particle* World::look_for_particle_underneath(Particle* particle) {
        ParticleIterator particle_iterator = this->iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return nullptr;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        Coordinate underneath_coordinates{
                particle_coordinates.first,
                particle_coordinates.second - 1
        };

        return particle_at(underneath_coordinates);
}

World::ParticleIterator World::iterator_of(Particle* particle){
        if (particle == nullptr) {
                return particles.end();
        }

        auto coordinate_iterator = particle_coordinates_by_pointer.find(particle);
        if (coordinate_iterator == particle_coordinates_by_pointer.end()) {
                return particles.end();
        }

        ParticleIterator particle_iterator = particles.find(coordinate_iterator->second);
        if (particle_iterator == particles.end() || particle_iterator->second.particle.get() != particle) {
                particle_coordinates_by_pointer.erase(coordinate_iterator);
                return particles.end();
        }

        return particle_iterator;
}

void World::index_particle_at(Coordinate coordinate, const Particle* particle) {
        if (particle == nullptr || particle == &void_particle) {
                return;
        }

        particle_coordinates_by_pointer[particle] = coordinate;
}

void World::unindex_particle(const Particle* particle) {
        if (particle == nullptr) {
                return;
        }

        particle_coordinates_by_pointer.erase(particle);
}

void World::retire_particle(ParticleIterator particle_iterator) {
        if (particle_iterator == particles.end()) {
                return;
        }

        unindex_particle(particle_iterator->second.particle.get());
        retired_particles.push_back(std::move(particle_iterator->second.particle));
}

void World::replace_particle_at(Coordinate coordinate, std::unique_ptr<Particle> particle) {
        if (!coordinate_is_inside_world(coordinate) || particle == nullptr) {
                return;
        }

        Particle* new_particle = particle.get();
        ParticleState new_particle_state{
                std::move(particle),
                {0, 0},
                {0, -1}
        };

        ParticleIterator existing_particle_iterator = particles.find(coordinate);
        if (existing_particle_iterator != particles.end()) {
                retire_particle(existing_particle_iterator);
                existing_particle_iterator->second = std::move(new_particle_state);
                index_particle_at(coordinate, existing_particle_iterator->second.particle.get());
        } else {
                auto insertion_result = particles.emplace(coordinate, std::move(new_particle_state));
                index_particle_at(coordinate, insertion_result.first->second.particle.get());
        }

        record_change_at(coordinate, new_particle);
}

void World::erase_particle(ParticleIterator particle_iterator) {
        if (particle_iterator == particles.end()) {
                return;
        }

        retire_particle(particle_iterator);
        particles.erase(particle_iterator);
}

Particle* World::look_for_particle_underneath(Coordinate particle_coordinates){
        Coordinate under_coordinates{
                particle_coordinates.first,
                particle_coordinates.second - 1
        };
        return particle_at(under_coordinates);
}

void World::move_particle_to(ParticleIterator particle_iterator, Coordinate new_coordinate) {
        if (particle_iterator == particles.end() || !coordinate_is_inside_world(new_coordinate)) {
                return;
        }

        const Coordinate old_coordinate = particle_iterator->first;
        if (old_coordinate == new_coordinate) {
                return;
        }

        ParticleState particle_state = std::move(particle_iterator->second);
        Particle* moved_particle = particle_state.particle.get();
        unindex_particle(moved_particle);
        particles.erase(particle_iterator);
        record_change_at(old_coordinate, &void_particle);

        ParticleIterator displaced_particle_iterator = particles.find(new_coordinate);
        if (displaced_particle_iterator != particles.end()) {
                erase_particle(displaced_particle_iterator);
        }

        auto insertion_result = particles.emplace(new_coordinate, std::move(particle_state));
        index_particle_at(new_coordinate, insertion_result.first->second.particle.get());
        record_change_at(new_coordinate, moved_particle);
}

void World::applyForce(Particle* particle, Vector force) {
        ParticleIterator particle_iterator = iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        ParticleState& particle_state = particle_iterator->second;
        particle_state.acceleration.x += force.x;
        particle_state.acceleration.y += force.y;
}

void World::apply_acceleration_to_speed(Particle* particle) {
        ParticleIterator particle_iterator = iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        ParticleState& particle_state = particle_iterator->second;
        particle_state.speed.x += particle_state.acceleration.x;
        particle_state.speed.y += particle_state.acceleration.y;
}

void World::reset_acceleration(Particle* particle) {
        ParticleIterator particle_iterator = iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        particle_iterator->second.acceleration = {0, -1};
}

void World::reset_speed(Particle* particle) {
        ParticleIterator particle_iterator = iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        particle_iterator->second.speed = {0, 0};
}

void World::particle_clash(Particle* clashing_particle, Particle* blocking_particle) {
        ParticleIterator clashing_particle_iterator = iterator_of(clashing_particle);
        if (clashing_particle_iterator == particles.end()) {
                return;
        }

        if (iterator_of(blocking_particle) == particles.end()) {
                return;
        }

        const Vector force = clashing_particle_iterator->second.speed;
        applyForce(blocking_particle, force);
        reset_speed(clashing_particle);
}

void World::water_pushing_onto_water(WaterParticle* pushing_water, WaterParticle* pushed_water) {
        ParticleIterator pushing_water_iterator = iterator_of(pushing_water);
        if (pushing_water_iterator == particles.end()) {
                return;
        }

        ParticleIterator pushed_water_iterator = iterator_of(pushed_water);
        if (pushed_water_iterator == particles.end()) {
                return;
        }

        const Vector speed = pushing_water_iterator->second.speed;
        if (speed.y < 0) {
                const Coordinate pushed_water_coordinate = pushed_water_iterator->first;
                const bool try_left_first = speed.x <= 0;

                WaterParticle* preferred_edge = try_left_first ?
                        left_water_edge(pushed_water) :
                        right_water_edge(pushed_water);
                Particle* preferred_target = try_left_first ?
                        look_for_particle_to_the_left_of(preferred_edge) :
                        look_for_particle_to_the_right_of(preferred_edge);

                const bool preferred_target_can_move = try_left_first ?
                        can_be_moved_to_the_left(preferred_edge) :
                        can_be_moved_to_the_right(preferred_edge);

                if (preferred_target != nullptr && preferred_target_can_move) {
                        if (try_left_first) {
                                move_water_chain_to_the_left(pushed_water);
                        } else {
                                move_water_chain_to_the_right(pushed_water);
                        }

                        pushing_water_iterator = iterator_of(pushing_water);
                        move_particle_to(pushing_water_iterator, pushed_water_coordinate);
                        return;
                }

                WaterParticle* fallback_edge = try_left_first ?
                        right_water_edge(pushed_water) :
                        left_water_edge(pushed_water);
                Particle* fallback_target = try_left_first ?
                        look_for_particle_to_the_right_of(fallback_edge) :
                        look_for_particle_to_the_left_of(fallback_edge);

                const bool fallback_target_can_move = try_left_first ?
                        can_be_moved_to_the_right(fallback_edge) :
                        can_be_moved_to_the_left(fallback_edge);

                if (fallback_target != nullptr && fallback_target_can_move) {
                        if (try_left_first) {
                                move_water_chain_to_the_right(pushed_water);
                        } else {
                                move_water_chain_to_the_left(pushed_water);
                        }

                        pushing_water_iterator = iterator_of(pushing_water);
                        move_particle_to(pushing_water_iterator, pushed_water_coordinate);
                        return;
                }

                reset_vertical_speed(pushing_water);
                return;
        }

        if (speed.y != 0) {
                applyForce(pushed_water, {0, speed.y});
                reset_speed(pushing_water);
                return;
        }

        if (speed.x == 0) {
                reset_speed(pushing_water);
                return;
        }

        apply_horizontal_water_wave_force(pushed_water, {speed.x, 0});
        reset_speed(pushing_water);
}

void World::apply_horizontal_water_wave_force(WaterParticle* pushed_water, Vector force) {
        const int horizontal_force = force.x;
        if (horizontal_force == 0) {
                return;
        }

        const bool force_points_left = horizontal_force < 0;
        WaterParticle* preferred_edge = force_points_left ?
                left_water_edge(pushed_water) :
                right_water_edge(pushed_water);
        Particle* preferred_blocking_particle = force_points_left ?
                look_for_particle_to_the_left_of(preferred_edge) :
                look_for_particle_to_the_right_of(preferred_edge);

        if (preferred_blocking_particle != nullptr &&
                        preferred_edge->can_pass_through_it(preferred_blocking_particle)) {
                applyForce(preferred_edge, {horizontal_force, 0});
                return;
        }

        WaterParticle* fallback_edge = force_points_left ?
                right_water_edge(pushed_water) :
                left_water_edge(pushed_water);
        Particle* fallback_blocking_particle = force_points_left ?
                look_for_particle_to_the_right_of(fallback_edge) :
                look_for_particle_to_the_left_of(fallback_edge);
        const int fallback_force = -horizontal_force;

        if (fallback_blocking_particle != nullptr &&
                        fallback_edge->can_pass_through_it(fallback_blocking_particle)) {
                applyForce(fallback_edge, {fallback_force, 0});
        }
}

void World::water_pushing_onto_solid(WaterParticle* water, Particle* solid) {
        ParticleIterator water_iterator = iterator_of(water);
        if (water_iterator == particles.end()) {
                return;
        }

        if (iterator_of(solid) == particles.end()) {
                return;
        }

        ParticleState& water_state = water_iterator->second;
        water_state.speed.x = -water_state.speed.x;
        water_state.speed.y = 0;
}

void World::water_pushing_onto_wood(WaterParticle* water_particle, WoodParticle* wood_particle) {
        ParticleIterator water_iterator = iterator_of(water_particle);
        if (water_iterator == particles.end()) {
                return;
        }

        if (iterator_of(wood_particle) == particles.end()) {
                return;
        }

        const Vector impact = water_iterator->second.speed;
        if (impact.y < 0) {
                return;
        }

        applyForce(wood_particle, impact);
        reset_speed(water_particle);
}

void World::water_pushing_onto_fire(WaterParticle* water_particle, FireParticle* fire_particle) {
        if (iterator_of(water_particle) == particles.end()) {
                return;
        }

        if (iterator_of(fire_particle) == particles.end()) {
                return;
        }

        fire_died(fire_particle);
}

void World::fire_pushing_onto_water(FireParticle* fire_particle, WaterParticle* water_particle) {
        if (iterator_of(fire_particle) == particles.end()) {
                return;
        }

        if (iterator_of(water_particle) == particles.end()) {
                return;
        }

        fire_died(fire_particle);
}

void World::crash_onto_wall(Particle* particle, int attempted_x, int attempted_y) {
        ParticleIterator particle_iterator = iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        ParticleState& particle_state = particle_iterator->second;
        if (attempted_x < 0 || attempted_x >= world_width) {
                particle_state.speed.x = -particle_state.speed.x;
        }

        if (attempted_y < 0 || attempted_y >= world_height) {
                particle_state.speed.y = 0;
        }
}

void World::reset_vertical_speed(Particle* particle) {
        ParticleIterator particle_iterator = iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        particle_iterator->second.speed.y = 0;
}

void World::reduce_vertical_speed(Particle* particle) {
        ParticleIterator particle_iterator = iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        ParticleState& particle_state = particle_iterator->second;
        if (particle_state.speed.y > 0) {
                particle_state.speed.y = (particle_state.speed.y + 1) / 2;
        } else {
                particle_state.speed.y /= 2;
        }
}

void World::apply_friction_to(Particle* particle) {
        ParticleIterator particle_iterator = iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        ParticleState& particle_state = particle_iterator->second;
        if (particle_state.speed.x > 0) {
                --particle_state.acceleration.x;
        } else if (particle_state.speed.x < 0) {
                ++particle_state.acceleration.x;
        }
}

void World::apply_movement_to(Particle* particle) {
        ParticleIterator particle_iterator = iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        const Vector movement = particle_iterator->second.speed;

        const Coordinate current_coordinate = particle_iterator->first;
        const Coordinate target_coordinate{
                current_coordinate.first + movement.x,
                current_coordinate.second + movement.y
        };

        const Coordinate maximum_coordinate = maximum_uninterrupted_movement_to(
                particle,
                movement
        );

        const bool movement_was_interrupted = maximum_coordinate != target_coordinate;

        if (movement_was_interrupted) {
                std::vector<Coordinate> cells_to_traverse = this->coordinates_vector_passes_through_starting_at(current_coordinate, movement);
                Coordinate blocking_coordinate = target_coordinate;

                for (std::size_t cell_index = 0; cell_index < cells_to_traverse.size(); ++cell_index) {
                        if (cells_to_traverse[cell_index] == maximum_coordinate && cell_index + 1 < cells_to_traverse.size()) {
                                blocking_coordinate = cells_to_traverse[cell_index + 1];
                                break;
                        }
                }

                if (maximum_coordinate != current_coordinate) {
                        move_particle_to(particle_iterator, maximum_coordinate);
                }

                if (!coordinate_is_inside_world(blocking_coordinate)) {
                        crash_onto_wall(particle, blocking_coordinate.first, blocking_coordinate.second);
                        if (movement.x != 0) {
                                apply_friction_to(particle);
                        }
                        return;
                }

                Particle* blocking_particle = particle_at(blocking_coordinate);
                if (blocking_particle != nullptr) {
                        blocking_particle->particle_is_pushing_onto(particle);
                }
        }else{

                move_particle_to(particle_iterator, maximum_coordinate);
        }

        if (movement.x != 0) {
                apply_friction_to(particle);
        }
}

std::vector<World::Coordinate> World::coordinates_vector_passes_through_starting_at(Coordinate starting_point, Vector trayectory){

        if(std::abs(trayectory.x) >= std::abs(trayectory.y)){
                return this->coordinates_wide_vector_passes_through_starting_at(starting_point, trayectory);
        }else{
                return this->coordinates_tall_vector_passes_through_starting_at(starting_point, trayectory);
        }
}

std::vector<World::Coordinate> World::coordinates_wide_vector_passes_through_starting_at(Coordinate starting_point, Vector trayectory){

        std::vector<Coordinate> cells;

        const int horizontal_direction = trayectory.x < 0 ? -1 : 1;
        const int vertical_direction = trayectory.y < 0 ? -1 : 1;

        if (trayectory.y == 0) {

                for (int i = 0; i <= std::abs(trayectory.x); i++) {
                        Coordinate horizontal_line_coordinate = {starting_point.first + i * horizontal_direction, starting_point.second};
                        cells.push_back(horizontal_line_coordinate);
                }
                return cells;
        }

        float ratio = std::abs((float)trayectory.x / trayectory.y);
        float current_ratio = 1.0f;
        int horizontal_steps = 1;
        int vertical_steps = 1;

        Coordinate current_coordinate = {starting_point.first, starting_point.second};
        Coordinate target_coordinate = {starting_point.first + trayectory.x, starting_point.second + trayectory.y};
        cells.push_back(current_coordinate);

        while(current_coordinate != target_coordinate){
                if(current_ratio < ratio){

                        current_coordinate.first += horizontal_direction;
                        cells.push_back(current_coordinate);
                        horizontal_steps++;

                }else{

                        current_coordinate.first += horizontal_direction;
                        current_coordinate.second += vertical_direction;
                        cells.push_back(current_coordinate);
                        horizontal_steps++;
                        vertical_steps++;
                }
                current_ratio = (float)horizontal_steps / vertical_steps;
        }

        return cells;

}
std::vector<World::Coordinate> World::coordinates_tall_vector_passes_through_starting_at(Coordinate starting_point, Vector trayectory){

        std::vector<Coordinate> cells;

        const int horizontal_direction = trayectory.x < 0 ? -1 : 1;
        const int vertical_direction = trayectory.y < 0 ? -1 : 1;

        if (trayectory.x == 0) {

                for (int i = 0; i <= std::abs(trayectory.y); i++) {
                        Coordinate vertical_line_coordinate = {starting_point.first, starting_point.second + i * vertical_direction};
                        cells.push_back(vertical_line_coordinate);
                }
                return cells;
        }

        float ratio = std::abs((float)trayectory.x / trayectory.y);
        float current_ratio = 1.0f;
        int horizontal_steps = 1;
        int vertical_steps = 1;

        Coordinate current_coordinate = {starting_point.first, starting_point.second};
        Coordinate target_coordinate = {starting_point.first + trayectory.x, starting_point.second + trayectory.y};
        cells.push_back(current_coordinate);

        while(current_coordinate != target_coordinate){
                if(current_ratio > ratio){

                        current_coordinate.second += vertical_direction;
                        cells.push_back(current_coordinate);
                        vertical_steps++;

                }else{

                        current_coordinate.first += horizontal_direction;
                        current_coordinate.second += vertical_direction;
                        cells.push_back(current_coordinate);
                        horizontal_steps++;
                        vertical_steps++;
                }

                current_ratio = (float)horizontal_steps/vertical_steps;
        }

        return cells;

}

World::Coordinate World::maximum_uninterrupted_movement_to(Particle* particle, Vector movement) {
        ParticleIterator particle_iterator = iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return {0, 0};
        }

        Coordinate maximum_coordinate = particle_iterator->first;
        std::vector<Coordinate> cells_to_traverse = this->coordinates_vector_passes_through_starting_at(maximum_coordinate, movement);

        for (Coordinate& cell: cells_to_traverse) {
                if (!coordinate_is_inside_world(cell)) {
                        return maximum_coordinate;
                }

                Particle* next_particle = particle_at(cell);
                if (next_particle != particle) {

                        if (next_particle == nullptr || !particle->can_pass_through_it(next_particle)) {
                                return maximum_coordinate;
                        }

                        maximum_coordinate = cell;

                }

        }

        return maximum_coordinate;
}

int World::maximum_distance_it_can_travel_to_the_left_capped_at(Particle* particle, int distance) {
        ParticleIterator particle_iterator = iterator_of(particle);
        if (particle_iterator == particles.end() || distance <= 0) {
                return 0;
        }

        int maximum_distance = 0;
        Coordinate coordinate = particle_iterator->first;
        for (int current_distance = 0; current_distance < distance; ++current_distance) {
                Coordinate next_coordinate{
                        coordinate.first - 1,
                        coordinate.second
                };
                if (!coordinate_is_inside_world(next_coordinate)) {
                        return maximum_distance;
                }

                Particle* next_particle = particle_at(next_coordinate);
                if (next_particle == nullptr || !particle->can_pass_through_it(next_particle)) {
                        return maximum_distance;
                }

                ++maximum_distance;
                coordinate = next_coordinate;
        }

        return maximum_distance;
}

int World::maximum_distance_it_can_travel_to_the_right_capped_at(Particle* particle, int distance) {
        ParticleIterator particle_iterator = iterator_of(particle);
        if (particle_iterator == particles.end() || distance <= 0) {
                return 0;
        }

        int maximum_distance = 0;
        Coordinate coordinate = particle_iterator->first;
        for (int current_distance = 0; current_distance < distance; ++current_distance) {
                Coordinate next_coordinate{
                        coordinate.first + 1,
                        coordinate.second
                };
                if (!coordinate_is_inside_world(next_coordinate)) {
                        return maximum_distance;
                }

                Particle* next_particle = particle_at(next_coordinate);
                if (next_particle == nullptr || !particle->can_pass_through_it(next_particle)) {
                        return maximum_distance;
                }

                ++maximum_distance;
                coordinate = next_coordinate;
        }

        return maximum_distance;
}

bool World::can_be_moved_to_the_left(Particle* particle) {
        Particle* particle_to_the_left = this->look_for_particle_to_the_left_of(particle);
        if (particle_to_the_left == nullptr) {
                return false;
        }

        if (particle_to_the_left->can_be_pushed_into_by_water()) {
                return true;
        }

        return particle_to_the_left->water_can_push_it_to_the_left();
}

bool World::can_be_moved_to_the_right(Particle* particle) {
        Particle* particle_to_the_right = this->look_for_particle_to_the_right_of(particle);
        if (particle_to_the_right == nullptr) {
                return false;
        }

        if (particle_to_the_right->can_be_pushed_into_by_water()) {
                return true;
        }

        return particle_to_the_right->water_can_push_it_to_the_right();
}

int World::amount_of_water_to_the_left_of(Particle* particle) {
        Particle* particle_to_the_left = this->look_for_particle_to_the_left_of(particle);
        if (particle_to_the_left == nullptr || !particle_to_the_left->isWater()) {
                return 0;
        }

        return particle_to_the_left->water_from_to_the_left();
}

int World::amount_of_water_to_the_right_of(Particle* particle) {
        Particle* particle_to_the_right = this->look_for_particle_to_the_right_of(particle);
        if (particle_to_the_right == nullptr || !particle_to_the_right->isWater()) {
                return 0;
        }

        return particle_to_the_right->water_from_to_the_right();
}

void World::move_water_chain_to_the_left(Particle* particle) {
        ParticleIterator particle_iterator = this->iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        Coordinate left_coordinates{
                particle_coordinates.first - 1,
                particle_coordinates.second
        };

        Particle* particle_to_the_left = this->look_for_particle_to_the_left_of(particle);
        if (particle_to_the_left == nullptr) {
                return;
        }

        if (!particle_to_the_left->can_be_pushed_into_by_water()) {
                if (!particle_to_the_left->water_can_push_it_to_the_left()) {
                        return;
                }

                move_water_chain_to_the_left(particle_to_the_left);
        }

        particle_iterator = this->iterator_of(particle);
        move_particle_to(particle_iterator, left_coordinates);
}

void World::move_water_chain_to_the_right(Particle* particle) {
        ParticleIterator particle_iterator = this->iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        Coordinate right_coordinates{
                particle_coordinates.first + 1,
                particle_coordinates.second
        };

        Particle* particle_to_the_right = this->look_for_particle_to_the_right_of(particle);
        if (particle_to_the_right == nullptr) {
                return;
        }

        if (!particle_to_the_right->can_be_pushed_into_by_water()) {
                if (!particle_to_the_right->water_can_push_it_to_the_right()) {
                        return;
                }

                move_water_chain_to_the_right(particle_to_the_right);
        }

        particle_iterator = this->iterator_of(particle);
        move_particle_to(particle_iterator, right_coordinates);
}

WaterParticle* World::left_water_edge(WaterParticle* water_particle) {
        WaterParticle* edge_water = water_particle;
        Particle* particle_to_the_left = this->look_for_particle_to_the_left_of(edge_water);

        while (particle_to_the_left != nullptr && particle_to_the_left->can_make_waves()) {
                edge_water = static_cast<WaterParticle*>(particle_to_the_left);
                particle_to_the_left = this->look_for_particle_to_the_left_of(edge_water);
        }

        return edge_water;
}

WaterParticle* World::right_water_edge(WaterParticle* water_particle) {
        WaterParticle* edge_water = water_particle;
        Particle* particle_to_the_right = this->look_for_particle_to_the_right_of(edge_water);

        while (particle_to_the_right != nullptr && particle_to_the_right->can_make_waves()) {
                edge_water = static_cast<WaterParticle*>(particle_to_the_right);
                particle_to_the_right = this->look_for_particle_to_the_right_of(edge_water);
        }

        return edge_water;
}

bool World::water_can_be_pushed_upwards(WaterParticle* water_particle) {
        ParticleIterator particle_iterator = this->iterator_of(water_particle);
        if (particle_iterator == particles.end()) {
                return false;
        }

        Particle* particle_above = this->look_for_particle_above(water_particle);
        if (particle_above == nullptr) {
                return false;
        }

        return particle_above->can_be_pushed_into_by_water() ||
                particle_above->water_can_push_it_upwards();
}

bool World::isDamStructureUpwards(Particle* particle) {
        Particle* particle_above = this->look_for_particle_above(particle);
        if (particle_above == nullptr) {
                return true;
        }

        return particle_above->isDamUpwards();
}

bool World::isDamStructureDownwards(Particle* particle) {
        Particle* particle_underneath = this->look_for_particle_underneath(particle);
        if (particle_underneath == nullptr) {
                return true;
        }

        return particle_underneath->isDamDownwards();
}

void World::move_water_chain_upwards(WaterParticle* water_particle) {
        ParticleIterator particle_iterator = this->iterator_of(water_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        Coordinate above_coordinates{
                particle_coordinates.first,
                particle_coordinates.second + 1
        };

        Particle* particle_above = this->look_for_particle_above(water_particle);
        if (particle_above == nullptr) {
                return;
        }

        if (!particle_above->can_be_pushed_into_by_water()) {
                if (!particle_above->water_can_push_it_upwards()) {
                        return;
                }

                move_water_chain_upwards(static_cast<WaterParticle*>(particle_above));
        }

        particle_iterator = this->iterator_of(water_particle);
        move_particle_to(particle_iterator, above_coordinates);
}

bool World::floatable_column_can_move_upwards(Particle* particle) {
        ParticleIterator particle_iterator = this->iterator_of(particle);
        if (particle_iterator == particles.end() || !particle->can_float_on_water()) {
                return false;
        }

        Particle* particle_above = this->look_for_particle_above(particle);
        if (particle_above == nullptr) {
                return false;
        }

        if (particle_above->can_float_on_water()) {
                return floatable_column_can_move_upwards(particle_above);
        }

        return particle->can_pass_through_it(particle_above);
}

bool World::move_floatable_column_upwards(Particle* particle) {
        ParticleIterator particle_iterator = this->iterator_of(particle);
        if (particle_iterator == particles.end() || !particle->can_float_on_water()) {
                return false;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        Coordinate above_coordinates{
                particle_coordinates.first,
                particle_coordinates.second + 1
        };

        Particle* particle_above = this->look_for_particle_above(particle);
        if (particle_above == nullptr) {
                return false;
        }

        if (particle_above->can_float_on_water()) {
                if (!move_floatable_column_upwards(particle_above)) {
                        return false;
                }
        } else if (!particle->can_pass_through_it(particle_above)) {
                return false;
        }

        particle_iterator = this->iterator_of(particle);
        move_particle_to(particle_iterator, above_coordinates);
        return true;
}

int World::distance_to_overflow_to_the_left_from(WaterParticle* water_particle) {
        if (iterator_of(water_particle) == particles.end()) {
                return -1;
        }

        WaterParticle* current_water = water_particle;
        int horizontal_distance = 0;

        while (true) {
                Particle* particle_to_the_left = this->look_for_particle_to_the_left_of(current_water);
                if (particle_to_the_left == nullptr || !particle_to_the_left->can_make_waves()) {
                        return -1;
                }

                WaterParticle* next_water = static_cast<WaterParticle*>(particle_to_the_left);
                ++horizontal_distance;

                Coordinate target_coordinate{0, 0};
                if (overflow_target_above(next_water, target_coordinate)) {
                        return horizontal_distance;
                }

                current_water = next_water;
        }
}

int World::distance_to_overflow_to_the_right_from(WaterParticle* water_particle) {
        if (iterator_of(water_particle) == particles.end()) {
                return -1;
        }

        WaterParticle* current_water = water_particle;
        int horizontal_distance = 0;

        while (true) {
                Particle* particle_to_the_right = this->look_for_particle_to_the_right_of(current_water);
                if (particle_to_the_right == nullptr || !particle_to_the_right->can_make_waves()) {
                        return -1;
                }

                WaterParticle* next_water = static_cast<WaterParticle*>(particle_to_the_right);
                ++horizontal_distance;

                Coordinate target_coordinate{0, 0};
                if (overflow_target_above(next_water, target_coordinate)) {
                        return horizontal_distance;
                }

                current_water = next_water;
        }
}

void World::support_stone(StoneParticle* stone_particle) {
        ParticleIterator particle_iterator = this->iterator_of(stone_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        for (int x_offset = -1; x_offset <= 1; ++x_offset) {
                for (int y_offset = -1; y_offset <= 1; ++y_offset) {
                        if (x_offset == 0 && y_offset == 0) {
                                continue;
                        }

                        Particle* neighbour = this->particle_at({
                                particle_coordinates.first + x_offset,
                                particle_coordinates.second + y_offset
                        });

                        if (neighbour != nullptr) {
                                neighbour->support(stone_particle);
                        }
                }
        }
}

bool World::water_can_overflow(WaterParticle* water_particle) {
        if (iterator_of(water_particle) == particles.end()) {
                return false;
        }

        return water_can_overflow_to_the_left_from(water_particle) ||
                water_can_overflow_to_the_right_from(water_particle);
}

bool World::water_can_overflow_to_the_left_from(WaterParticle* water_particle) {
        Coordinate target_coordinate{0, 0};
        return overflow_target_to_the_left_from(water_particle, target_coordinate);
}

bool World::water_can_overflow_to_the_right_from(WaterParticle* water_particle) {
        Coordinate target_coordinate{0, 0};
        return overflow_target_to_the_right_from(water_particle, target_coordinate);
}

bool World::overflow_target_above(WaterParticle* water_particle, Coordinate& target_coordinate) {
        WaterParticle* current_water = water_particle;

        while (true) {
                ParticleIterator current_water_iterator = iterator_of(current_water);
                if (current_water_iterator == particles.end()) {
                        return false;
                }

                Coordinate above_coordinate{
                        current_water_iterator->first.first,
                        current_water_iterator->first.second + 1
                };
                Particle* particle_above = particle_at(above_coordinate);
                if (particle_above == nullptr) {
                        return false;
                }

                if (particle_above->can_make_waves()) {
                        current_water = static_cast<WaterParticle*>(particle_above);
                        continue;
                }

                if (current_water->can_pass_through_it(particle_above)) {
                        target_coordinate = above_coordinate;
                        return true;
                }

                if (particle_above->can_float_on_water() && floatable_column_can_move_upwards(particle_above)) {
                        target_coordinate = above_coordinate;
                        return true;
                }

                return false;
        }
}

bool World::overflow_target_to_the_left_from(WaterParticle* water_particle, Coordinate& target_coordinate) {
        WaterParticle* current_water = water_particle;

        while (true) {
                Particle* particle_to_the_left = this->look_for_particle_to_the_left_of(current_water);
                if (particle_to_the_left == nullptr || !particle_to_the_left->can_make_waves()) {
                        return false;
                }

                WaterParticle* next_water = static_cast<WaterParticle*>(particle_to_the_left);
                if (overflow_target_above(next_water, target_coordinate)) {
                        return true;
                }

                current_water = next_water;
        }
}

bool World::overflow_target_to_the_right_from(WaterParticle* water_particle, Coordinate& target_coordinate) {
        WaterParticle* current_water = water_particle;

        while (true) {
                Particle* particle_to_the_right = this->look_for_particle_to_the_right_of(current_water);
                if (particle_to_the_right == nullptr || !particle_to_the_right->can_make_waves()) {
                        return false;
                }

                WaterParticle* next_water = static_cast<WaterParticle*>(particle_to_the_right);
                if (overflow_target_above(next_water, target_coordinate)) {
                        return true;
                }

                current_water = next_water;
        }
}

bool World::make_water_overflow(WaterParticle* clashed_water, Vector impact) {
        if (iterator_of(clashed_water) == particles.end()) {
                return false;
        }

        const int left_distance = distance_to_overflow_to_the_left_from(clashed_water);
        const int right_distance = distance_to_overflow_to_the_right_from(clashed_water);

        if (left_distance == -1 && right_distance == -1) {
                return false;
        }

        const bool overflow_to_the_left =
                left_distance != -1 &&
                (right_distance == -1 || left_distance <= right_distance);

        Coordinate target_coordinate{0, 0};
        const bool can_overflow = overflow_to_the_left ?
                overflow_target_to_the_left_from(clashed_water, target_coordinate) :
                overflow_target_to_the_right_from(clashed_water, target_coordinate);

        if (!can_overflow) {
                return false;
        }

        Particle* target_particle = particle_at(target_coordinate);
        if (target_particle != nullptr && target_particle->can_float_on_water()) {
                if (!move_floatable_column_upwards(target_particle)) {
                        return false;
                }
        }

        ParticleIterator clashed_water_iterator = iterator_of(clashed_water);
        move_particle_to(clashed_water_iterator, target_coordinate);

        const int horizontal_direction = overflow_to_the_left ? -1 : 1;
        applyForce(clashed_water, {
                horizontal_direction * std::abs(impact.x),
                -impact.y
        });

        return true;
}

void World::particle_pushing_onto_void(Particle* particle, VoidParticle* void_particle){

        ParticleIterator particle_iterator = this->iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        ParticleIterator void_iterator = this->iterator_of(void_particle);
        if (void_iterator == particles.end()) {
                return;
        }

        Coordinate new_coordinate = void_iterator->first;

        move_particle_to(particle_iterator, new_coordinate);
}

void World::dirt_particle_pushing_onto_dirt(DirtParticle* pushing_dirt, DirtParticle* blocking_dirt){
        ParticleIterator particle_iterator = this->iterator_of(pushing_dirt);
        if (particle_iterator == particles.end()) {
                return;
        }

        if (iterator_of(blocking_dirt) == particles.end()) {
                return;
        }

        if (particle_iterator->second.speed.y == 0) {
                particle_clash(pushing_dirt, blocking_dirt);
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        if (particle_coordinates.first != 0) {
                Coordinate left_coordinates{
                        particle_coordinates.first - 1,
                        particle_coordinates.second
                };

                Particle* particle_to_the_left = this->look_for_particle_to_the_left_of(pushing_dirt);
                Particle* particle_underneath_to_the_left = this->look_for_particle_underneath(left_coordinates);

                if(particle_to_the_left != nullptr && particle_underneath_to_the_left != nullptr &&
                                particle_to_the_left->dirt_can_push_through() &&
                                particle_underneath_to_the_left->dirt_can_push_through()){
                        this->make_particle_fall_to_the_left(pushing_dirt, particle_underneath_to_the_left);
                        return;
                }
        }

        if (particle_coordinates.first < world_width - 1) {
                Coordinate right_coordinates{
                        particle_coordinates.first + 1,
                        particle_coordinates.second
                };
                Particle* particle_to_the_right = this->look_for_particle_to_the_right_of(pushing_dirt);
                Particle* particle_underneath_to_the_right = this->look_for_particle_underneath(right_coordinates);

                if(particle_to_the_right != nullptr && particle_underneath_to_the_right != nullptr &&
                                particle_to_the_right->dirt_can_push_through() &&
                                particle_underneath_to_the_right->dirt_can_push_through()){
                        this->make_particle_fall_to_the_right(pushing_dirt, particle_underneath_to_the_right);
                        return;
                }
        }

        reset_vertical_speed(pushing_dirt);
}

void World::wood_pushing_onto_water(WoodParticle* wood_particle, WaterParticle* water_particle){

        ParticleIterator particle_iterator = this->iterator_of(wood_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        ParticleIterator water_iterator = this->iterator_of(water_particle);
        if (water_iterator == particles.end()) {
                return;
        }

        const Vector impact = particle_iterator->second.speed;
        if (impact.y >= 0) {
                particle_clash(wood_particle, water_particle);
                return;
        }else{
                applyForce(water_particle, {impact.x, 0});
                reset_speed(wood_particle);
        }

        


}

void World::solid_pushing_onto_water(Particle* particle, WaterParticle* water_particle){

        ParticleIterator particle_iterator = this->iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        ParticleIterator water_iterator = this->iterator_of(water_particle);
        if (water_iterator == particles.end()) {
                return;
        }

        const Vector impact = particle_iterator->second.speed;
        if (impact.y == 0) {
                particle_clash(particle, water_particle);
                return;
        }

        const Coordinate water_coordinates = water_iterator->first;
        const bool try_left_first = impact.x <= 0;

        WaterParticle* preferred_edge = try_left_first ?
                left_water_edge(water_particle) :
                right_water_edge(water_particle);
        Particle* preferred_target = try_left_first ?
                look_for_particle_to_the_left_of(preferred_edge) :
                look_for_particle_to_the_right_of(preferred_edge);

        const bool preferred_target_can_move = try_left_first ?
                can_be_moved_to_the_left(preferred_edge) :
                can_be_moved_to_the_right(preferred_edge);

        if (preferred_target != nullptr && preferred_target_can_move) {
                if (try_left_first) {
                        move_water_chain_to_the_left(water_particle);
                } else {
                        move_water_chain_to_the_right(water_particle);
                }

                applyForce(preferred_edge, impact);
                particle_iterator = this->iterator_of(particle);
                move_particle_to(particle_iterator, water_coordinates);
                reduce_vertical_speed(particle);
                return;
        }

        WaterParticle* fallback_edge = try_left_first ?
                right_water_edge(water_particle) :
                left_water_edge(water_particle);
        Particle* fallback_target = try_left_first ?
                look_for_particle_to_the_right_of(fallback_edge) :
                look_for_particle_to_the_left_of(fallback_edge);

        const bool fallback_target_can_move = try_left_first ?
                can_be_moved_to_the_right(fallback_edge) :
                can_be_moved_to_the_left(fallback_edge);

        if (fallback_target != nullptr && fallback_target_can_move) {
                if (try_left_first) {
                        move_water_chain_to_the_right(water_particle);
                } else {
                        move_water_chain_to_the_left(water_particle);
                }

                applyForce(fallback_edge, {-impact.x, impact.y});
                particle_iterator = this->iterator_of(particle);
                move_particle_to(particle_iterator, water_coordinates);
                reduce_vertical_speed(particle);
                return;
        }

        if (water_particle->can_overflow() && make_water_overflow(water_particle, impact)) {
                particle_iterator = this->iterator_of(particle);
                move_particle_to(particle_iterator, water_coordinates);
                reduce_vertical_speed(particle);
        }
}

void World::turn_dirt_into_mud(DirtParticle* dirt_particle) {
        ParticleIterator dirt_iterator = this->iterator_of(dirt_particle);
        if (dirt_iterator == particles.end()) {
                return;
        }

        const Coordinate dirt_coordinate = dirt_iterator->first;
        replace_particle_at(dirt_coordinate, std::make_unique<MudParticle>(this));
}



void World::mud_particle_pushing_onto_water(MudParticle* mud_particle, WaterParticle* water_particle){
        solid_pushing_onto_water(mud_particle, water_particle);
}

void World::stone_particle_pushing_onto_void(StoneParticle* stone_particle){
        ParticleIterator particle_iterator = this->iterator_of(stone_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;

        Coordinate new_coordinate = particle_coordinates;
        --new_coordinate.second;

        move_particle_to(particle_iterator, new_coordinate);
}

void World::stone_particle_pushing_onto_water(StoneParticle* stone_particle, WaterParticle* water_particle){
        solid_pushing_onto_water(stone_particle, water_particle);
}

void World::wood_particle_pushing_onto_void(WoodParticle* wood_particle){
        ParticleIterator particle_iterator = this->iterator_of(wood_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;

        Coordinate new_coordinate = particle_coordinates;
        --new_coordinate.second;

        move_particle_to(particle_iterator, new_coordinate);
}

void World::fire_particle_pushing_onto_void(FireParticle* fire_particle){
        ParticleIterator particle_iterator = this->iterator_of(fire_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;

        Coordinate new_coordinate = particle_coordinates;
        --new_coordinate.second;

        move_particle_to(particle_iterator, new_coordinate);
}

void World::grass_particle_pushing_onto_water(GrassParticle* grass_particle, WaterParticle* water_particle){
        solid_pushing_onto_water(grass_particle, water_particle);
}

void World::water_particle_pushing_onto_dirt(WaterParticle* water_particle, DirtParticle* dirt_particle){
        ParticleIterator water_iterator = this->iterator_of(water_particle);
        if (water_iterator == particles.end()) {
                return;
        }

        ParticleIterator dirt_iterator = this->iterator_of(dirt_particle);
        if (dirt_iterator == particles.end()) {
                return;
        }

        Coordinate dirt_coordinate = dirt_iterator->first;
        Coordinate water_coordinate = water_iterator->first;
        replace_particle_at(dirt_coordinate, std::make_unique<MudParticle>(this));
        water_iterator = this->iterator_of(water_particle);
        erase_particle(water_iterator);
        record_change_at(water_coordinate, &void_particle);
}

void World::dirt_falling_to_the_left_onto_void(DirtParticle* dirt_particle, VoidParticle* void_particle){
        ParticleIterator particle_iterator = this->iterator_of(dirt_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        move_particle_to(particle_iterator, {particle_coordinates.first - 1, particle_coordinates.second - 1});
}

void World::dirt_falling_to_the_right_onto_void(DirtParticle* dirt_particle, VoidParticle* void_particle){
        ParticleIterator particle_iterator = this->iterator_of(dirt_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        move_particle_to(particle_iterator, {particle_coordinates.first + 1, particle_coordinates.second - 1});
}

void World::dirt_falling_to_the_left_onto_fire(DirtParticle* dirt_particle, FireParticle* fire_particle){
        ParticleIterator particle_iterator = this->iterator_of(dirt_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        fire_died(fire_particle);
        move_particle_to(particle_iterator, {particle_coordinates.first - 1, particle_coordinates.second - 1});
}

void World::dirt_falling_to_the_right_onto_fire(DirtParticle* dirt_particle, FireParticle* fire_particle){
        ParticleIterator particle_iterator = this->iterator_of(dirt_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        fire_died(fire_particle);
        move_particle_to(particle_iterator, {particle_coordinates.first + 1, particle_coordinates.second - 1});
}

void World::mud_particle_pushing_onto_void(MudParticle* mud_particle){
        ParticleIterator particle_iterator = this->iterator_of(mud_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;

        Coordinate new_coordinate = particle_coordinates;
        --new_coordinate.second;

        move_particle_to(particle_iterator, new_coordinate);
}

void World::mud_particle_pushing_onto_mud(MudParticle* pushing_mud, MudParticle* blocking_mud){
        ParticleIterator particle_iterator = this->iterator_of(pushing_mud);
        if (particle_iterator == particles.end()) {
                return;
        }

        if (iterator_of(blocking_mud) == particles.end()) {
                return;
        }

        if (particle_iterator->second.speed.y == 0) {
                particle_clash(pushing_mud, blocking_mud);
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;

        if (particle_coordinates.first != 0) {
                Coordinate left_coordinates{
                        particle_coordinates.first - 1,
                        particle_coordinates.second
                };

                Particle* particle_to_the_left = this->look_for_particle_to_the_left_of(pushing_mud);
                Particle* particle_underneath_to_the_left = this->look_for_particle_underneath(left_coordinates);

                if(particle_to_the_left != nullptr && particle_underneath_to_the_left != nullptr &&
                                particle_to_the_left->dirt_can_push_through() &&
                                particle_underneath_to_the_left->dirt_can_push_through()){
                        particle_underneath_to_the_left->mud_pushing_to_the_left(pushing_mud);
                        return;
                }
        }

        if (particle_coordinates.first < world_width - 1) {
                Coordinate right_coordinates{
                        particle_coordinates.first + 1,
                        particle_coordinates.second
                };
                Particle* particle_to_the_right = this->look_for_particle_to_the_right_of(pushing_mud);
                Particle* particle_underneath_to_the_right = this->look_for_particle_underneath(right_coordinates);

                if(particle_to_the_right != nullptr && particle_underneath_to_the_right != nullptr &&
                                particle_to_the_right->dirt_can_push_through() &&
                                particle_underneath_to_the_right->dirt_can_push_through()){
                        particle_underneath_to_the_right->mud_pushing_to_the_right(pushing_mud);
                        return;
                }
        }

        reset_vertical_speed(pushing_mud);
}

void World::mud_falling_to_the_left_onto_void(MudParticle* mud_particle, VoidParticle* void_particle){
        ParticleIterator particle_iterator = this->iterator_of(mud_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        move_particle_to(particle_iterator, {particle_coordinates.first - 1, particle_coordinates.second - 1});
}

void World::mud_falling_to_the_right_onto_void(MudParticle* mud_particle, VoidParticle* void_particle){
        ParticleIterator particle_iterator = this->iterator_of(mud_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        move_particle_to(particle_iterator, {particle_coordinates.first + 1, particle_coordinates.second - 1});
}

void World::mud_falling_to_the_left_onto_fire(MudParticle* mud_particle, FireParticle* fire_particle){
        ParticleIterator particle_iterator = this->iterator_of(mud_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        fire_died(fire_particle);
        move_particle_to(particle_iterator, {particle_coordinates.first - 1, particle_coordinates.second - 1});
}

void World::mud_falling_to_the_right_onto_fire(MudParticle* mud_particle, FireParticle* fire_particle){
        ParticleIterator particle_iterator = this->iterator_of(mud_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        fire_died(fire_particle);
        move_particle_to(particle_iterator, {particle_coordinates.first + 1, particle_coordinates.second - 1});
}

void World::grass_particle_pushing_onto_void(GrassParticle* grass_particle){

        ParticleIterator particle_iterator = this->iterator_of(grass_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;

        Coordinate new_coordinate = particle_coordinates;
        --new_coordinate.second;

        move_particle_to(particle_iterator, new_coordinate);
}

void World::water_particle_pushing_onto_void(WaterParticle* water_particle){

        ParticleIterator particle_iterator = this->iterator_of(water_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;

        Coordinate new_coordinate = particle_coordinates;
        --new_coordinate.second;

        move_particle_to(particle_iterator, new_coordinate);
}

void World::water_particle_pushing_onto_blocking_particle(WaterParticle* water_particle){
        ParticleIterator particle_iterator = this->iterator_of(water_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;

        if (particle_coordinates.first != 0) {
                Coordinate left_coordinates{
                        particle_coordinates.first - 1,
                        particle_coordinates.second
                };

                Particle* particle_to_the_left = this->look_for_particle_to_the_left_of(water_particle);
                Particle* particle_underneath_to_the_left = this->look_for_particle_underneath(left_coordinates);

                if(particle_to_the_left != nullptr && particle_underneath_to_the_left != nullptr &&
                                particle_to_the_left->dirt_can_push_through() &&
                                particle_underneath_to_the_left->dirt_can_push_through()){
                        this->make_water_particle_fall_to_the_left(water_particle, particle_underneath_to_the_left);
                        return;
                }
        }

        if (particle_coordinates.first < world_width - 1) {
                Coordinate right_coordinates{
                        particle_coordinates.first + 1,
                        particle_coordinates.second
                };
                Particle* particle_to_the_right = this->look_for_particle_to_the_right_of(water_particle);
                Particle* particle_underneath_to_the_right = this->look_for_particle_underneath(right_coordinates);

                if(particle_to_the_right != nullptr && particle_underneath_to_the_right != nullptr &&
                                particle_to_the_right->dirt_can_push_through() &&
                                particle_underneath_to_the_right->dirt_can_push_through()){
                        this->make_water_particle_fall_to_the_right(water_particle, particle_underneath_to_the_right);
                        return;
                }
        }

        reset_vertical_speed(water_particle);
}

void World::water_falling_to_the_left_onto_void(WaterParticle* water_particle, VoidParticle* void_particle){
        ParticleIterator particle_iterator = this->iterator_of(water_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        move_particle_to(particle_iterator, {particle_coordinates.first - 1, particle_coordinates.second - 1});
}

void World::water_falling_to_the_right_onto_void(WaterParticle* water_particle, VoidParticle* void_particle){
        ParticleIterator particle_iterator = this->iterator_of(water_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        move_particle_to(particle_iterator, {particle_coordinates.first + 1, particle_coordinates.second - 1});
}

void World::grass_trying_to_spread(){
        for (ParticleIterator iterator = particles.begin(); iterator != particles.end(); ++iterator) {
                if (iterator->second.particle->isGrass()) {
                        this->grass_trying_to_spread(static_cast<GrassParticle*>(iterator->second.particle.get()));
                }
        }
}

void World::grass_trying_to_spread(GrassParticle* grass_particle){
        Particle* particle_to_the_left = this->look_for_particle_to_the_left_of(grass_particle);
        if (particle_to_the_left != nullptr) {
                particle_to_the_left->grass_trying_to_spread_onto();
        }

        Particle* particle_to_the_right = this->look_for_particle_to_the_right_of(grass_particle);
        if (particle_to_the_right != nullptr) {
                particle_to_the_right->grass_trying_to_spread_onto();
        }
}

void World::grass_spreads_onto(DirtParticle* dirt_particle){
        ParticleIterator particle_iterator = this->iterator_of(dirt_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinate = particle_iterator->first;
        replace_particle_at(particle_coordinate, std::make_unique<GrassParticle>(this));
}

void World::fire_trying_to_spread(FireParticle* fire_particle){
        ParticleIterator particle_iterator = this->iterator_of(fire_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;

        for (int x_offset = -1; x_offset <= 1; ++x_offset) {
                for (int y_offset = -1; y_offset <= 1; ++y_offset) {
                        if (x_offset == 0 && y_offset == 0) {
                                continue;
                        }

                        Particle* neighbour = this->particle_at({
                                particle_coordinates.first + x_offset,
                                particle_coordinates.second + y_offset
                        });

                        if (neighbour != nullptr) {
                                neighbour->burn();
                        }
                }
        }
}

void World::fire_spreading_onto(Particle* particle){
        ParticleIterator particle_iterator = this->iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinate = particle_iterator->first;
        replace_particle_at(particle_coordinate, std::make_unique<FireParticle>(this));
}

void World::fire_died(FireParticle* fire_particle){

        ParticleIterator particle_iterator = this->iterator_of(fire_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinate = particle_iterator->first;
        erase_particle(particle_iterator);
        record_change_at(particle_coordinate, &void_particle);
}
