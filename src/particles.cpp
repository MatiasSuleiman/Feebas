#include "particles.hpp"

#include "world.hpp"

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

bool DirtParticle::dirt_can_fall_through() const {
  return false;
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


void DirtParticle::dirt_falling_to_the_left(DirtParticle* dirt_particle){
}

void DirtParticle::dirt_falling_to_the_right(DirtParticle* dirt_particle){
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

bool VoidParticle::dirt_can_fall_through() const {
  return true;
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

void VoidParticle::dirt_falling_to_the_left(DirtParticle* dirt_particle){
        world_is_in->dirt_falling_to_the_left_onto_void(dirt_particle, this);
}

void VoidParticle::dirt_falling_to_the_right(DirtParticle* dirt_particle){
        world_is_in->dirt_falling_to_the_right_onto_void(dirt_particle, this);
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

bool GrassParticle::dirt_can_fall_through() const {
  return false;
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

void GrassParticle::dirt_falling_to_the_left(DirtParticle* dirt_particle){
}

void GrassParticle::dirt_falling_to_the_right(DirtParticle* dirt_particle){
}

void GrassParticle::grass_trying_to_spread_onto(){}
