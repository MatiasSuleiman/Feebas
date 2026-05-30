#include "particles.hpp"

#include "world.hpp"
#include "world_visitor.hpp"

Particle::Particle(World* world) : world_is_in(world) {
}

Particle::~Particle() {
}

DirtParticle::DirtParticle(World* world) : Particle(world) {
}

bool DirtParticle::isDirt() const {
  return true;
}

bool DirtParticle::isGrass() const {
  return false;
}

bool DirtParticle::isVoid() const {
  return false;
}

bool DirtParticle::isWater() const {
  return false;
}

bool DirtParticle::isMud() const {
  return false;
}

bool DirtParticle::dirt_can_fall_through() const {
  return false;
}

bool DirtParticle::can_be_pushed_into_by_water() const {
        return false;
}

bool DirtParticle::can_be_pushed_to_the_left() {
        return false;
}

bool DirtParticle::can_be_pushed_to_the_right() {
        return false;
}

int DirtParticle::water_to_the_left() {
        return 0;
}

int DirtParticle::water_to_the_right() {
        return 0;
}

int DirtParticle::water_from_to_the_left() {
        return 0;
}

int DirtParticle::water_from_to_the_right() {
        return 0;
}

void DirtParticle::accept(WorldVisitor& visitor) const {
        visitor.visit_dirt_particle(*this);
}

void DirtParticle::step() {
  world_is_in->make_particle_fall(this);
}

void DirtParticle::particle_is_falling_onto(Particle* particle){
        particle->fall_onto_dirt(this);
}

void DirtParticle::fall_onto_dirt(DirtParticle* dirt_particle){
        world_is_in->dirt_particle_falling_onto_dirt(this);
}

void DirtParticle::fall_onto_void(VoidParticle* void_particle){
        world_is_in->dirt_particle_falling_onto_void(this);
}

void DirtParticle::fall_onto_water(WaterParticle* water_particle){
        world_is_in->dirt_particle_falling_onto_water(this, water_particle);
}

void DirtParticle::fall_onto_mud(MudParticle* mud_particle){
}

void DirtParticle::dirt_falling_to_the_left(DirtParticle* dirt_particle){
}

void DirtParticle::dirt_falling_to_the_right(DirtParticle* dirt_particle){
}

void DirtParticle::mud_falling_to_the_left(MudParticle* mud_particle){
}

void DirtParticle::mud_falling_to_the_right(MudParticle* mud_particle){
}

void DirtParticle::water_falling_to_the_left(WaterParticle* water_particle){
}

void DirtParticle::water_falling_to_the_right(WaterParticle* water_particle){
}

void DirtParticle::grass_trying_to_spread_onto(){
        if(steps_until_being_grass > 0){
                steps_until_being_grass--;
        }

        if(steps_until_being_grass == 0){
                world_is_in->grass_spreads_onto(this);
        }
}

VoidParticle::VoidParticle(World* world) : Particle(world) {
}

bool VoidParticle::isDirt() const {
  return false;
}

bool VoidParticle::isGrass() const {
  return false;
}

bool VoidParticle::isVoid() const {
  return true;
}

bool VoidParticle::isWater() const {
  return false;
}

bool VoidParticle::isMud() const {
  return false;
}

bool VoidParticle::dirt_can_fall_through() const {
  return true;
}

bool VoidParticle::can_be_pushed_into_by_water() const {
        return true;
}

bool VoidParticle::can_be_pushed_to_the_left() {
        return false;
}

bool VoidParticle::can_be_pushed_to_the_right() {
        return false;
}

int VoidParticle::water_to_the_left() {
        return 0;
}

int VoidParticle::water_to_the_right() {
        return 0;
}

int VoidParticle::water_from_to_the_left() {
        return 0;
}

int VoidParticle::water_from_to_the_right() {
        return 0;
}

void VoidParticle::accept(WorldVisitor& visitor) const {
        visitor.visit_void_particle(*this);
}

void VoidParticle::step() {
}

void VoidParticle::particle_is_falling_onto(Particle* particle){
        particle->fall_onto_void(this);
}

void VoidParticle::fall_onto_dirt(DirtParticle* dirt_particle){
}

void VoidParticle::fall_onto_void(VoidParticle* void_particle){
}

void VoidParticle::fall_onto_water(WaterParticle* water_particle){
}

void VoidParticle::fall_onto_mud(MudParticle* mud_particle){
}

void VoidParticle::dirt_falling_to_the_left(DirtParticle* dirt_particle){
        world_is_in->dirt_falling_to_the_left_onto_void(dirt_particle, this);
}

void VoidParticle::dirt_falling_to_the_right(DirtParticle* dirt_particle){
        world_is_in->dirt_falling_to_the_right_onto_void(dirt_particle, this);
}

void VoidParticle::mud_falling_to_the_left(MudParticle* mud_particle){
        world_is_in->mud_falling_to_the_left_onto_void(mud_particle, this);
}

void VoidParticle::mud_falling_to_the_right(MudParticle* mud_particle){
        world_is_in->mud_falling_to_the_right_onto_void(mud_particle, this);
}

void VoidParticle::water_falling_to_the_left(WaterParticle* water_particle){
        world_is_in->water_falling_to_the_left_onto_void(water_particle, this);
}

void VoidParticle::water_falling_to_the_right(WaterParticle* water_particle){
        world_is_in->water_falling_to_the_right_onto_void(water_particle, this);
}

void VoidParticle::grass_trying_to_spread_onto(){}

GrassParticle::GrassParticle(World* world) : Particle(world) {
}

bool GrassParticle::isDirt() const {
  return false;
}

bool GrassParticle::isGrass() const {
  return true;
}

bool GrassParticle::isVoid() const {
  return false;
}

bool GrassParticle::isWater() const {
  return false;
}

bool GrassParticle::isMud() const {
  return false;
}

bool GrassParticle::dirt_can_fall_through() const {
  return false;
}

bool GrassParticle::can_be_pushed_into_by_water() const {
        return false;
}

bool GrassParticle::can_be_pushed_to_the_left() {
        return false;
}

bool GrassParticle::can_be_pushed_to_the_right() {
        return false;
}

int GrassParticle::water_to_the_left() {
        return 0;
}

int GrassParticle::water_to_the_right() {
        return 0;
}

int GrassParticle::water_from_to_the_left() {
        return 0;
}

int GrassParticle::water_from_to_the_right() {
        return 0;
}

void GrassParticle::accept(WorldVisitor& visitor) const {
        visitor.visit_grass_particle(*this);
}

void GrassParticle::step() {
        world_is_in->make_particle_fall(this);
        world_is_in->grass_trying_to_spread();
}

void GrassParticle::particle_is_falling_onto(Particle* particle){
}

void GrassParticle::fall_onto_dirt(DirtParticle* dirt_particle){
}

void GrassParticle::fall_onto_void(VoidParticle* void_particle){
        world_is_in->grass_particle_falling_onto_void(this);
}

void GrassParticle::fall_onto_water(WaterParticle* water_particle){
}

void GrassParticle::fall_onto_mud(MudParticle* mud_particle){
}

void GrassParticle::dirt_falling_to_the_left(DirtParticle* dirt_particle){
}

void GrassParticle::dirt_falling_to_the_right(DirtParticle* dirt_particle){
}

void GrassParticle::mud_falling_to_the_left(MudParticle* mud_particle){
}

void GrassParticle::mud_falling_to_the_right(MudParticle* mud_particle){
}

void GrassParticle::water_falling_to_the_left(WaterParticle* water_particle){
}

void GrassParticle::water_falling_to_the_right(WaterParticle* water_particle){
}

void GrassParticle::grass_trying_to_spread_onto(){}

WaterParticle::WaterParticle(World* world) : Particle(world) {
}

bool WaterParticle::isDirt() const {
  return false;
}

bool WaterParticle::isGrass() const {
  return false;
}

bool WaterParticle::isVoid() const {
  return false;
}

bool WaterParticle::isWater() const {
  return true;
}

bool WaterParticle::isMud() const {
  return false;
}

bool WaterParticle::dirt_can_fall_through() const {
  return false;
}

bool WaterParticle::can_be_pushed_into_by_water() const {
        return false;
}

bool WaterParticle::can_be_pushed_to_the_left() {
        return world_is_in->can_be_moved_to_the_left(this);
}

bool WaterParticle::can_be_pushed_to_the_right() {
        return world_is_in->can_be_moved_to_the_right(this);
}

int WaterParticle::water_to_the_left() {
        Particle* particle = world_is_in->particle_to_the_left(this);
        if (particle == nullptr) {
                return 0;
        }

        return particle->water_from_to_the_left();
}

int WaterParticle::water_to_the_right() {
        Particle* particle = world_is_in->particle_to_the_right(this);
        if (particle == nullptr) {
                return 0;
        }

        return particle->water_from_to_the_right();
}

int WaterParticle::water_from_to_the_left() {
        Particle* particle = world_is_in->particle_to_the_left(this);
        if (particle == nullptr) {
                return 1;
        }

        return 1 + particle->water_from_to_the_left();
}

int WaterParticle::water_from_to_the_right() {
        Particle* particle = world_is_in->particle_to_the_right(this);
        if (particle == nullptr) {
                return 1;
        }

        return 1 + particle->water_from_to_the_right();
}

void WaterParticle::accept(WorldVisitor& visitor) const {
        visitor.visit_water_particle(*this);
}

void WaterParticle::step() {
        world_is_in->make_particle_fall(this);
}

void WaterParticle::particle_is_falling_onto(Particle* particle){
        particle->fall_onto_water(this);
}

void WaterParticle::fall_onto_dirt(DirtParticle* dirt_particle){
        world_is_in->water_particle_falling_onto_dirt(this, dirt_particle);
}

void WaterParticle::fall_onto_void(VoidParticle* void_particle){
        world_is_in->water_particle_falling_onto_void(this);
}

void WaterParticle::fall_onto_water(WaterParticle* water_particle){
        world_is_in->water_particle_falling_onto_water(this, water_particle);
}

void WaterParticle::fall_onto_mud(MudParticle* mud_particle){
}

void WaterParticle::dirt_falling_to_the_left(DirtParticle* dirt_particle){
}

void WaterParticle::dirt_falling_to_the_right(DirtParticle* dirt_particle){
}

void WaterParticle::mud_falling_to_the_left(MudParticle* mud_particle){
}

void WaterParticle::mud_falling_to_the_right(MudParticle* mud_particle){
}

void WaterParticle::water_falling_to_the_left(WaterParticle* water_particle){
}

void WaterParticle::water_falling_to_the_right(WaterParticle* water_particle){
}

void WaterParticle::grass_trying_to_spread_onto(){}

MudParticle::MudParticle(World* world) : Particle(world) {
}

bool MudParticle::isDirt() const {
  return false;
}

bool MudParticle::isGrass() const {
  return false;
}

bool MudParticle::isVoid() const {
  return false;
}

bool MudParticle::isWater() const {
  return false;
}

bool MudParticle::isMud() const {
  return true;
}

bool MudParticle::dirt_can_fall_through() const {
  return false;
}

bool MudParticle::can_be_pushed_into_by_water() const {
        return false;
}

bool MudParticle::can_be_pushed_to_the_left() {
        return false;
}

bool MudParticle::can_be_pushed_to_the_right() {
        return false;
}

int MudParticle::water_to_the_left() {
        return 0;
}

int MudParticle::water_to_the_right() {
        return 0;
}

int MudParticle::water_from_to_the_left() {
        return 0;
}

int MudParticle::water_from_to_the_right() {
        return 0;
}

void MudParticle::accept(WorldVisitor& visitor) const {
        visitor.visit_mud_particle(*this);
}

void MudParticle::step() {
        world_is_in->make_particle_fall(this);
}

void MudParticle::particle_is_falling_onto(Particle* particle){
        particle->fall_onto_mud(this);
}

void MudParticle::fall_onto_dirt(DirtParticle* dirt_particle){
        world_is_in->mud_particle_falling_onto_blocking_particle(this);
}

void MudParticle::fall_onto_void(VoidParticle* void_particle){
        world_is_in->mud_particle_falling_onto_void(this);
}

void MudParticle::fall_onto_water(WaterParticle* water_particle){
}

void MudParticle::fall_onto_mud(MudParticle* mud_particle){
        world_is_in->mud_particle_falling_onto_blocking_particle(this);
}

void MudParticle::dirt_falling_to_the_left(DirtParticle* dirt_particle){
}

void MudParticle::dirt_falling_to_the_right(DirtParticle* dirt_particle){
}

void MudParticle::mud_falling_to_the_left(MudParticle* mud_particle){
}

void MudParticle::mud_falling_to_the_right(MudParticle* mud_particle){
}

void MudParticle::water_falling_to_the_left(WaterParticle* water_particle){
}

void MudParticle::water_falling_to_the_right(WaterParticle* water_particle){
}

void MudParticle::grass_trying_to_spread_onto(){
}
