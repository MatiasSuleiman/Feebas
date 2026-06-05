#include "world.hpp"

#include <algorithm>
#include <memory>
#include <vector>

#include "world_to_json_visitor.hpp"

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
        particle_iterator->second->accept(visitor);
      } else {
        void_particle.accept(visitor);
      }
    }
  }

  visitor.finish_visiting_world();
}

std::string World::to_json() const {
  WorldToJSONVisitor visitor;
  accept(visitor);
  return visitor.json();
}

void World::Create_dirt_particle_at(int x, int y) {
  particles.insert_or_assign({x, y}, std::make_unique<DirtParticle>(this));
}

void World::Create_grass_particle_at(int x, int y) {
  particles.insert_or_assign({x, y}, std::make_unique<GrassParticle>(this));
}

void World::Create_water_particle_at(int x, int y) {
  particles.insert_or_assign({x, y}, std::make_unique<WaterParticle>(this));
}

void World::Create_mud_particle_at(int x, int y) {
  particles.insert_or_assign({x, y}, std::make_unique<MudParticle>(this));
}

void World::Create_stone_particle_at(int x, int y) {
  particles.insert_or_assign({x, y}, std::make_unique<StoneParticle>(this));
}

void World::Create_wood_particle_at(int x, int y) {
  particles.insert_or_assign({x, y}, std::make_unique<WoodParticle>(this));
}

void World::Create_fire_particle_at(int x, int y) {
  particles.insert_or_assign({x, y}, std::make_unique<FireParticle>(this));
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


void World::make_particle_fall(Particle* particle) {
       
        ParticleIterator iterator = this->iterator_of(particle);
        if (iterator == particles.end()) {
                return;
        }

        const Coordinate old_coordinate = iterator->first;

        if(old_coordinate.second > 0){
                Particle* particle_underneath = this->look_for_particle_underneath(iterator->second.get());
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

void World::make_water_particle_fall_to_the_left(WaterParticle* water_particle, Particle* bottom_left_edge_particle) {
        bottom_left_edge_particle->water_falling_to_the_left(water_particle);
}

void World::make_water_particle_fall_to_the_right(WaterParticle* water_particle, Particle* bottom_right_edge_particle) {
        bottom_right_edge_particle->water_falling_to_the_right(water_particle);
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
                        iterator->second->step();
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

        return target_iterator->second.get();
}

const Particle* World::particle_at(Coordinate coordinate) const {
        if (!coordinate_is_inside_world(coordinate)) {
                return nullptr;
        }

        ConstParticleIterator target_iterator = particles.find(coordinate);
        if (target_iterator == particles.end()) {
                return &void_particle;
        }

        return target_iterator->second.get();
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
        return particle_at(under_coordinates);
}

void World::move_particle_to(ParticleIterator particle_iterator, Coordinate new_coordinate) {
        if (particle_iterator == particles.end() || !coordinate_is_inside_world(new_coordinate)) {
                return;
        }

        std::unique_ptr<Particle> particle = std::move(particle_iterator->second);
        particles.erase(particle_iterator);
        particles.insert_or_assign(new_coordinate, std::move(particle));
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

int World::distance_to_overflow_to_the_left_from(WaterParticle* water_particle) {
        ParticleIterator particle_iterator = this->iterator_of(water_particle);
        if (particle_iterator == particles.end()) {
                return -1;
        }

        if (water_particle->water_can_push_it_upwards()) {
                return 0;
        }

        Particle* particle_to_the_left = this->look_for_particle_to_the_left_of(water_particle);
        if (particle_to_the_left == nullptr ||
                        (!particle_to_the_left->water_can_push_it_upwards() &&
                         !particle_to_the_left->water_can_push_it_to_the_left())) {
                return -1;
        }

        int distance = distance_to_overflow_to_the_left_from(static_cast<WaterParticle*>(particle_to_the_left));
        if (distance == -1) {
                return -1;
        }

        return 1 + distance;
}

int World::distance_to_overflow_to_the_right_from(WaterParticle* water_particle) {
        ParticleIterator particle_iterator = this->iterator_of(water_particle);
        if (particle_iterator == particles.end()) {
                return -1;
        }

        if (water_particle->water_can_push_it_upwards()) {
                return 0;
        }

        Particle* particle_to_the_right = this->look_for_particle_to_the_right_of(water_particle);
        if (particle_to_the_right == nullptr ||
                        (!particle_to_the_right->water_can_push_it_upwards() &&
                         !particle_to_the_right->water_can_push_it_to_the_right())) {
                return -1;
        }

        int distance = distance_to_overflow_to_the_right_from(static_cast<WaterParticle*>(particle_to_the_right));
        if (distance == -1) {
                return -1;
        }

        return 1 + distance;
}

bool World::has_stone_upwards(Particle* particle) {
        Particle* particle_above = this->look_for_particle_above(particle);
        return particle_above != nullptr && particle_above->isStone();
}

bool World::has_stone_downwards(Particle* particle) {
        Particle* particle_underneath = this->look_for_particle_underneath(particle);
        return particle_underneath != nullptr && particle_underneath->isStone();
}

bool World::has_stone_to_the_left(Particle* particle) {
        Particle* particle_to_the_left = this->look_for_particle_to_the_left_of(particle);
        return particle_to_the_left != nullptr && particle_to_the_left->isStone();
}

bool World::has_stone_to_the_right(Particle* particle) {
        Particle* particle_to_the_right = this->look_for_particle_to_the_right_of(particle);
        return particle_to_the_right != nullptr && particle_to_the_right->isStone();
}

bool World::has_stone_to_the_upper_left(Particle* particle) {
        ParticleIterator particle_iterator = this->iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return false;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        Particle* particle_to_the_upper_left = this->particle_at({
                particle_coordinates.first - 1,
                particle_coordinates.second + 1
        });
        return particle_to_the_upper_left != nullptr && particle_to_the_upper_left->isStone();
}

bool World::has_stone_to_the_upper_right(Particle* particle) {
        ParticleIterator particle_iterator = this->iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return false;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        Particle* particle_to_the_upper_right = this->particle_at({
                particle_coordinates.first + 1,
                particle_coordinates.second + 1
        });
        return particle_to_the_upper_right != nullptr && particle_to_the_upper_right->isStone();
}

bool World::has_stone_to_the_lower_left(Particle* particle) {
        ParticleIterator particle_iterator = this->iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return false;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        Particle* particle_to_the_lower_left = this->particle_at({
                particle_coordinates.first - 1,
                particle_coordinates.second - 1
        });
        return particle_to_the_lower_left != nullptr && particle_to_the_lower_left->isStone();
}

bool World::has_stone_to_the_lower_right(Particle* particle) {
        ParticleIterator particle_iterator = this->iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return false;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        Particle* particle_to_the_lower_right = this->particle_at({
                particle_coordinates.first + 1,
                particle_coordinates.second - 1
        });
        return particle_to_the_lower_right != nullptr && particle_to_the_lower_right->isStone();
}

bool World::water_chain_can_overflow_to_the_left(Particle* particle) {
        if (particle == nullptr || !particle->isWater()) {
                return false;
        }

        if (particle->water_can_push_it_upwards()) {
                return true;
        }

        return water_chain_can_overflow_to_the_left(this->look_for_particle_to_the_left_of(particle));
}

bool World::water_chain_can_overflow_to_the_right(Particle* particle) {
        if (particle == nullptr || !particle->isWater()) {
                return false;
        }

        if (particle->water_can_push_it_upwards()) {
                return true;
        }

        return water_chain_can_overflow_to_the_right(this->look_for_particle_to_the_right_of(particle));
}

void World::move_water_chain_overflow_to_the_left(WaterParticle* water_particle) {
        ParticleIterator particle_iterator = this->iterator_of(water_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        Coordinate left_coordinates{
                particle_coordinates.first - 1,
                particle_coordinates.second
        };

        Particle* particle_to_the_left = this->look_for_particle_to_the_left_of(water_particle);
        if (particle_to_the_left == nullptr) {
                return;
        }

        if (particle_to_the_left->water_can_push_it_upwards()) {
                move_water_chain_upwards(static_cast<WaterParticle*>(particle_to_the_left));
        } else {
                move_water_chain_overflow_to_the_left(static_cast<WaterParticle*>(particle_to_the_left));
        }

        particle_iterator = this->iterator_of(water_particle);
        move_particle_to(particle_iterator, left_coordinates);
}

void World::move_water_chain_overflow_to_the_right(WaterParticle* water_particle) {
        ParticleIterator particle_iterator = this->iterator_of(water_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;
        Coordinate right_coordinates{
                particle_coordinates.first + 1,
                particle_coordinates.second
        };

        Particle* particle_to_the_right = this->look_for_particle_to_the_right_of(water_particle);
        if (particle_to_the_right == nullptr) {
                return;
        }

        if (particle_to_the_right->water_can_push_it_upwards()) {
                move_water_chain_upwards(static_cast<WaterParticle*>(particle_to_the_right));
        } else {
                move_water_chain_overflow_to_the_right(static_cast<WaterParticle*>(particle_to_the_right));
        }

        particle_iterator = this->iterator_of(water_particle);
        move_particle_to(particle_iterator, right_coordinates);
}



void World::dirt_particle_falling_onto_void(DirtParticle* dirt_particle){
        ParticleIterator particle_iterator = this->iterator_of(dirt_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;

        Coordinate new_coordinate = particle_coordinates;
        --new_coordinate.second;

        move_particle_to(particle_iterator, new_coordinate);
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

                Particle* particle_to_the_left = this->look_for_particle_to_the_left_of(dirt_particle);
                Particle* particle_underneath_to_the_left = this->look_for_particle_underneath(left_coordinates);

                if(particle_to_the_left != nullptr && particle_underneath_to_the_left != nullptr &&
                                particle_to_the_left->dirt_can_fall_through() &&
                                particle_underneath_to_the_left->dirt_can_fall_through()){
                        this->make_particle_fall_to_the_left(dirt_particle, particle_underneath_to_the_left);
                        return;
                }
        }

        if (particle_coordinates.first < world_width - 1) {
                Coordinate right_coordinates{
                        particle_coordinates.first + 1,
                        particle_coordinates.second
                };
                Particle* particle_to_the_right = this->look_for_particle_to_the_right_of(dirt_particle);
                Particle* particle_underneath_to_the_right = this->look_for_particle_underneath(right_coordinates);

                if(particle_to_the_right != nullptr && particle_underneath_to_the_right != nullptr &&
                                particle_to_the_right->dirt_can_fall_through() &&
                                particle_underneath_to_the_right->dirt_can_fall_through()){
                        this->make_particle_fall_to_the_right(dirt_particle, particle_underneath_to_the_right);
                }
        }
}

void World::solid_falling_onto_water(Particle* particle, WaterParticle* water_particle){

        ParticleIterator particle_iterator = this->iterator_of(particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        ParticleIterator water_iterator = this->iterator_of(water_particle);
        if (water_iterator == particles.end()) {
                return;
        }

        Coordinate water_coordinates = water_iterator->first;
        bool water_can_be_pushed_to_the_left = water_particle->water_can_push_it_to_the_left();
        bool water_can_be_pushed_to_the_right = water_particle->water_can_push_it_to_the_right();

        if (water_can_be_pushed_to_the_left && water_can_be_pushed_to_the_right) {
                if (water_particle->water_to_the_left() <= water_particle->water_to_the_right()) {
                        move_water_chain_to_the_left(water_particle);
                } else {
                        move_water_chain_to_the_right(water_particle);
                }
        } else if (water_can_be_pushed_to_the_left) {
                move_water_chain_to_the_left(water_particle);
        } else if (water_can_be_pushed_to_the_right) {
                move_water_chain_to_the_right(water_particle);
        }

        if (water_can_be_pushed_to_the_left || water_can_be_pushed_to_the_right) {
                particle_iterator = this->iterator_of(particle);
                move_particle_to(particle_iterator, water_coordinates);
                return;
        }

        Particle* particle_to_the_left = this->look_for_particle_to_the_left_of(water_particle);
        Particle* particle_to_the_right = this->look_for_particle_to_the_right_of(water_particle);
        bool can_overflow_to_the_left = water_chain_can_overflow_to_the_left(particle_to_the_left);
        bool can_overflow_to_the_right = water_chain_can_overflow_to_the_right(particle_to_the_right);

        if (can_overflow_to_the_left && can_overflow_to_the_right) {
                if (water_particle->left_has_the_nearest_space_to_overlflow()) {
                        move_water_chain_overflow_to_the_left(water_particle);
                } else {
                        move_water_chain_overflow_to_the_right(water_particle);
                }

                particle_iterator = this->iterator_of(particle);
                move_particle_to(particle_iterator, water_coordinates);
                return;
        }

        if (can_overflow_to_the_left) {
                move_water_chain_overflow_to_the_left(water_particle);

                particle_iterator = this->iterator_of(particle);
                move_particle_to(particle_iterator, water_coordinates);
                return;
        }

        if (can_overflow_to_the_right) {
                move_water_chain_overflow_to_the_right(water_particle);

                particle_iterator = this->iterator_of(particle);
                move_particle_to(particle_iterator, water_coordinates);
        }
}

void World::dirt_particle_falling_onto_water(DirtParticle* dirt_particle, WaterParticle* water_particle){
        solid_falling_onto_water(dirt_particle, water_particle);

        ParticleIterator dirt_iterator = this->iterator_of(dirt_particle);
        if (dirt_iterator == particles.end()) {
                return;
        }

        dirt_iterator->second = std::make_unique<MudParticle>(this);
}

void World::mud_particle_falling_onto_water(MudParticle* mud_particle, WaterParticle* water_particle){
        solid_falling_onto_water(mud_particle, water_particle);
}

void World::stone_particle_falling_onto_void(StoneParticle* stone_particle){
        ParticleIterator particle_iterator = this->iterator_of(stone_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;

        Coordinate new_coordinate = particle_coordinates;
        --new_coordinate.second;

        move_particle_to(particle_iterator, new_coordinate);
}

void World::stone_particle_falling_onto_water(StoneParticle* stone_particle, WaterParticle* water_particle){
        solid_falling_onto_water(stone_particle, water_particle);
}

void World::wood_particle_falling_onto_void(WoodParticle* wood_particle){
        ParticleIterator particle_iterator = this->iterator_of(wood_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;

        Coordinate new_coordinate = particle_coordinates;
        --new_coordinate.second;

        move_particle_to(particle_iterator, new_coordinate);
}

void World::fire_particle_falling_onto_void(FireParticle* fire_particle){
        ParticleIterator particle_iterator = this->iterator_of(fire_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;

        Coordinate new_coordinate = particle_coordinates;
        --new_coordinate.second;

        move_particle_to(particle_iterator, new_coordinate);
}

void World::grass_particle_falling_onto_water(GrassParticle* grass_particle, WaterParticle* water_particle){
        solid_falling_onto_water(grass_particle, water_particle);
}

void World::water_particle_falling_onto_dirt(WaterParticle* water_particle, DirtParticle* dirt_particle){
        ParticleIterator water_iterator = this->iterator_of(water_particle);
        if (water_iterator == particles.end()) {
                return;
        }

        ParticleIterator dirt_iterator = this->iterator_of(dirt_particle);
        if (dirt_iterator == particles.end()) {
                return;
        }

        dirt_iterator->second = std::make_unique<MudParticle>(this);
        particles.erase(water_iterator);
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

void World::mud_particle_falling_onto_void(MudParticle* mud_particle){
        ParticleIterator particle_iterator = this->iterator_of(mud_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;

        Coordinate new_coordinate = particle_coordinates;
        --new_coordinate.second;

        move_particle_to(particle_iterator, new_coordinate);
}

void World::mud_particle_falling_onto_blocking_particle(MudParticle* mud_particle){
        ParticleIterator particle_iterator = this->iterator_of(mud_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;

        if (particle_coordinates.first != 0) {
                Coordinate left_coordinates{
                        particle_coordinates.first - 1,
                        particle_coordinates.second
                };

                Particle* particle_to_the_left = this->look_for_particle_to_the_left_of(mud_particle);
                Particle* particle_underneath_to_the_left = this->look_for_particle_underneath(left_coordinates);

                if(particle_to_the_left != nullptr && particle_underneath_to_the_left != nullptr &&
                                particle_to_the_left->dirt_can_fall_through() &&
                                particle_underneath_to_the_left->dirt_can_fall_through()){
                        particle_underneath_to_the_left->mud_falling_to_the_left(mud_particle);
                        return;
                }
        }

        if (particle_coordinates.first < world_width - 1) {
                Coordinate right_coordinates{
                        particle_coordinates.first + 1,
                        particle_coordinates.second
                };
                Particle* particle_to_the_right = this->look_for_particle_to_the_right_of(mud_particle);
                Particle* particle_underneath_to_the_right = this->look_for_particle_underneath(right_coordinates);

                if(particle_to_the_right != nullptr && particle_underneath_to_the_right != nullptr &&
                                particle_to_the_right->dirt_can_fall_through() &&
                                particle_underneath_to_the_right->dirt_can_fall_through()){
                        particle_underneath_to_the_right->mud_falling_to_the_right(mud_particle);
                }
        }
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

void World::grass_particle_falling_onto_void(GrassParticle* grass_particle){

        ParticleIterator particle_iterator = this->iterator_of(grass_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;

        Coordinate new_coordinate = particle_coordinates;
        --new_coordinate.second;

        move_particle_to(particle_iterator, new_coordinate);
}

void World::water_particle_falling_onto_void(WaterParticle* water_particle){

        ParticleIterator particle_iterator = this->iterator_of(water_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        Coordinate particle_coordinates = particle_iterator->first;

        Coordinate new_coordinate = particle_coordinates;
        --new_coordinate.second;

        move_particle_to(particle_iterator, new_coordinate);
}

void World::water_particle_falling_onto_blocking_particle(WaterParticle* water_particle){
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
                                particle_to_the_left->dirt_can_fall_through() &&
                                particle_underneath_to_the_left->dirt_can_fall_through()){
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
                                particle_to_the_right->dirt_can_fall_through() &&
                                particle_underneath_to_the_right->dirt_can_fall_through()){
                        this->make_water_particle_fall_to_the_right(water_particle, particle_underneath_to_the_right);
                }
        }
}

void World::water_particle_falling_onto_water(WaterParticle* falling_water_particle, WaterParticle* blocking_water_particle){
        ParticleIterator falling_water_iterator = this->iterator_of(falling_water_particle);
        if (falling_water_iterator == particles.end()) {
                return;
        }

        ParticleIterator blocking_water_iterator = this->iterator_of(blocking_water_particle);
        if (blocking_water_iterator == particles.end()) {
                return;
        }

        Coordinate blocking_water_coordinates = blocking_water_iterator->first;

        if (blocking_water_particle->water_can_push_it_to_the_left()) {
                if (blocking_water_particle->water_can_push_it_to_the_right()) {
                        if (blocking_water_particle->water_to_the_left() <= blocking_water_particle->water_to_the_right()) {
                                move_water_chain_to_the_left(blocking_water_particle);
                        } else {
                                move_water_chain_to_the_right(blocking_water_particle);
                        }
                } else {
                        move_water_chain_to_the_left(blocking_water_particle);
                }

                falling_water_iterator = this->iterator_of(falling_water_particle);
                move_particle_to(falling_water_iterator, blocking_water_coordinates);
                return;
        } else {
                if (blocking_water_particle->water_can_push_it_to_the_right()) {
                        move_water_chain_to_the_right(blocking_water_particle);
                        falling_water_iterator = this->iterator_of(falling_water_particle);
                        move_particle_to(falling_water_iterator, blocking_water_coordinates);
                }
        }
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
                if (iterator->second->isGrass()) {
                        this->grass_trying_to_spread(static_cast<GrassParticle*>(iterator->second.get()));
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

        particle_iterator->second = std::make_unique<GrassParticle>(this);
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

void World::fire_spreading_onto(GrassParticle* grass_particle){
        ParticleIterator particle_iterator = this->iterator_of(grass_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        particle_iterator->second = std::make_unique<FireParticle>(this);
}

void World::fire_died(FireParticle* fire_particle){

        ParticleIterator particle_iterator = this->iterator_of(fire_particle);
        if (particle_iterator == particles.end()) {
                return;
        }

        particles.erase(particle_iterator);
}
