#include "particles.hpp"

#include "world.hpp"
#include "world_visitor.hpp"

Particle::Particle(World* world) : world_is_in(world) {
}

Particle::~Particle() {
}

bool Particle::water_can_push_it_upwards() {
        return false;
}

bool Particle::isDam() {
        return false;
}

bool Particle::isDamUpwards() {
        return false;
}

bool Particle::isDamDownwards() {
        return false;
}

bool Particle::can_make_waves() {
        return false;
}

bool Particle::can_overflow() {
        return false;
}

bool Particle::can_float_on_water() {
        return false;
}

bool Particle::dirt_can_pass_through_it() {
        return false;
}

bool Particle::grass_can_pass_through_it() {
        return false;
}

bool Particle::water_can_pass_through_it() {
        return false;
}

bool Particle::mud_can_pass_through_it() {
        return false;
}

bool Particle::stone_can_pass_through_it() {
        return false;
}

bool Particle::wood_can_pass_through_it() {
        return false;
}

bool Particle::fire_can_pass_through_it() {
        return false;
}

void Particle::push_onto_dirt(DirtParticle* dirt_particle) {
        world_is_in->particle_clash(this, dirt_particle);
}

void Particle::push_onto_void(VoidParticle* void_particle) {
        world_is_in->particle_pushing_onto_void(this, void_particle);
}

void Particle::push_onto_water(WaterParticle* water_particle) {
        world_is_in->particle_clash(this, water_particle);
}

void Particle::push_onto_mud(MudParticle* mud_particle) {
        world_is_in->particle_clash(this, mud_particle);
}

void Particle::push_onto_grass(GrassParticle* grass_particle) {
        world_is_in->particle_clash(this, grass_particle);
}

void Particle::push_onto_stone(StoneParticle* stone_particle) {
        world_is_in->particle_clash(this, stone_particle);
}

void Particle::push_onto_wood(WoodParticle* wood_particle) {
        world_is_in->particle_clash(this, wood_particle);
}

void Particle::push_onto_fire(FireParticle* fire_particle) {
        world_is_in->particle_clash(this, fire_particle);
}

void Particle::support(StoneParticle* stone_particle) {
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

bool DirtParticle::isStone() const {
        return false;
}

bool DirtParticle::isWood() const {
        return false;
}

bool DirtParticle::isFire() const {
        return false;
}

bool DirtParticle::dirt_can_push_through() const {
  return false;
}

bool DirtParticle::can_pass_through_it(Particle* blocking_particle) {
        return blocking_particle->dirt_can_pass_through_it();
}

bool DirtParticle::can_be_pushed_into_by_water() const {
        return false;
}

bool DirtParticle::water_can_push_it_to_the_left() {
        return false;
}

bool DirtParticle::water_can_push_it_to_the_right() {
        return false;
}

bool DirtParticle::water_can_push_it_upwards() {
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
}

void DirtParticle::particle_is_pushing_onto(Particle* particle){
        particle->push_onto_dirt(this);
}

void DirtParticle::push_onto_dirt(DirtParticle* dirt_particle){
        world_is_in->dirt_particle_pushing_onto_dirt(this, dirt_particle);
}

void DirtParticle::push_onto_void(VoidParticle* void_particle){
        Particle::push_onto_void(void_particle);
}

void DirtParticle::push_onto_water(WaterParticle* water_particle){
        world_is_in->solid_pushing_onto_water(this, water_particle);
        world_is_in->turn_dirt_into_mud(this);
}

void DirtParticle::push_onto_mud(MudParticle* mud_particle){
        Particle::push_onto_mud(mud_particle);
}

void DirtParticle::push_onto_fire(FireParticle* fire_particle){
        Particle::push_onto_fire(fire_particle);
}

void DirtParticle::dirt_pushing_to_the_left(DirtParticle* dirt_particle){
}

void DirtParticle::dirt_pushing_to_the_right(DirtParticle* dirt_particle){
}

void DirtParticle::mud_pushing_to_the_left(MudParticle* mud_particle){
}

void DirtParticle::mud_pushing_to_the_right(MudParticle* mud_particle){
}

void DirtParticle::water_pushing_to_the_left(WaterParticle* water_particle){
}

void DirtParticle::water_pushing_to_the_right(WaterParticle* water_particle){
}

void DirtParticle::grass_trying_to_spread_onto(){
        if(steps_until_being_grass > 0){
                steps_until_being_grass--;
        }

        if(steps_until_being_grass == 0){
                world_is_in->grass_spreads_onto(this);
        }
}

void DirtParticle::burn(){
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

bool VoidParticle::isStone() const {
        return false;
}

bool VoidParticle::isWood() const {
        return false;
}

bool VoidParticle::isFire() const {
        return false;
}

bool VoidParticle::dirt_can_push_through() const {
  return true;
}

bool VoidParticle::can_pass_through_it(Particle* blocking_particle) {
        return true;
}

bool VoidParticle::dirt_can_pass_through_it() {
        return true;
}

bool VoidParticle::grass_can_pass_through_it() {
        return true;
}

bool VoidParticle::water_can_pass_through_it() {
        return true;
}

bool VoidParticle::mud_can_pass_through_it() {
        return true;
}

bool VoidParticle::stone_can_pass_through_it() {
        return true;
}

bool VoidParticle::wood_can_pass_through_it() {
        return true;
}

bool VoidParticle::fire_can_pass_through_it() {
        return true;
}

bool VoidParticle::can_be_pushed_into_by_water() const {
        return true;
}

bool VoidParticle::water_can_push_it_to_the_left() {
        return false;
}

bool VoidParticle::water_can_push_it_to_the_right() {
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

void VoidParticle::particle_is_pushing_onto(Particle* particle){
        particle->push_onto_void(this);
}

void VoidParticle::push_onto_dirt(DirtParticle* dirt_particle){
        Particle::push_onto_dirt(dirt_particle);
}

void VoidParticle::push_onto_void(VoidParticle* void_particle){
        Particle::push_onto_void(void_particle);
}

void VoidParticle::push_onto_water(WaterParticle* water_particle){
        Particle::push_onto_water(water_particle);
}

void VoidParticle::push_onto_mud(MudParticle* mud_particle){
        Particle::push_onto_mud(mud_particle);
}

void VoidParticle::dirt_pushing_to_the_left(DirtParticle* dirt_particle){
        world_is_in->dirt_falling_to_the_left_onto_void(dirt_particle, this);
}

void VoidParticle::dirt_pushing_to_the_right(DirtParticle* dirt_particle){
        world_is_in->dirt_falling_to_the_right_onto_void(dirt_particle, this);
}

void VoidParticle::mud_pushing_to_the_left(MudParticle* mud_particle){
        world_is_in->mud_falling_to_the_left_onto_void(mud_particle, this);
}

void VoidParticle::mud_pushing_to_the_right(MudParticle* mud_particle){
        world_is_in->mud_falling_to_the_right_onto_void(mud_particle, this);
}

void VoidParticle::water_pushing_to_the_left(WaterParticle* water_particle){
        world_is_in->water_falling_to_the_left_onto_void(water_particle, this);
}

void VoidParticle::water_pushing_to_the_right(WaterParticle* water_particle){
        world_is_in->water_falling_to_the_right_onto_void(water_particle, this);
}

void VoidParticle::grass_trying_to_spread_onto(){}

void VoidParticle::burn(){}

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

bool GrassParticle::isStone() const {
        return false;
}

bool GrassParticle::isWood() const {
        return false;
}

bool GrassParticle::isFire() const {
        return false;
}

bool GrassParticle::dirt_can_push_through() const {
  return false;
}

bool GrassParticle::can_pass_through_it(Particle* blocking_particle) {
        return blocking_particle->grass_can_pass_through_it();
}

bool GrassParticle::can_float_on_water() {
        return true;
}

bool GrassParticle::fire_can_pass_through_it() {
        return false;
}

bool GrassParticle::can_be_pushed_into_by_water() const {
        return false;
}

bool GrassParticle::water_can_push_it_to_the_left() {
        return false;
}

bool GrassParticle::water_can_push_it_to_the_right() {
        return false;
}

bool GrassParticle::water_can_push_it_upwards() {
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
        world_is_in->grass_trying_to_spread();
}

void GrassParticle::particle_is_pushing_onto(Particle* particle){
        particle->push_onto_grass(this);
}

void GrassParticle::push_onto_dirt(DirtParticle* dirt_particle){
        Particle::push_onto_dirt(dirt_particle);
}

void GrassParticle::push_onto_void(VoidParticle* void_particle){
        Particle::push_onto_void(void_particle);
}

void GrassParticle::push_onto_water(WaterParticle* water_particle){
        world_is_in->solid_pushing_onto_water(this, water_particle);
}

void GrassParticle::push_onto_mud(MudParticle* mud_particle){
        Particle::push_onto_mud(mud_particle);
}

void GrassParticle::push_onto_fire(FireParticle* fire_particle){
        Particle::push_onto_fire(fire_particle);
}

void GrassParticle::dirt_pushing_to_the_left(DirtParticle* dirt_particle){
}

void GrassParticle::dirt_pushing_to_the_right(DirtParticle* dirt_particle){
}

void GrassParticle::mud_pushing_to_the_left(MudParticle* mud_particle){
}

void GrassParticle::mud_pushing_to_the_right(MudParticle* mud_particle){
}

void GrassParticle::water_pushing_to_the_left(WaterParticle* water_particle){
}

void GrassParticle::water_pushing_to_the_right(WaterParticle* water_particle){
}

void GrassParticle::grass_trying_to_spread_onto(){}

void GrassParticle::burn(){
        steps_until_burning_out--;
        if(steps_until_burning_out <= 0){
                world_is_in->fire_spreading_onto(this);
        }
}

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

bool WaterParticle::isStone() const {
        return false;
}

bool WaterParticle::isWood() const {
        return false;
}

bool WaterParticle::isFire() const {
        return false;
}

bool WaterParticle::can_make_waves() {
        return true;
}

bool WaterParticle::can_overflow() {
        return world_is_in->water_can_overflow(this);
}

bool WaterParticle::dirt_can_push_through() const {
  return false;
}

bool WaterParticle::can_pass_through_it(Particle* blocking_particle) {
        return blocking_particle->water_can_pass_through_it();
}

bool WaterParticle::can_be_pushed_into_by_water() const {
        return false;
}

bool WaterParticle::water_can_push_it_to_the_left() {
        return world_is_in->can_be_moved_to_the_left(this);
}

bool WaterParticle::water_can_push_it_to_the_right() {
        return world_is_in->can_be_moved_to_the_right(this);
}

bool WaterParticle::water_can_push_it_upwards() {
        return world_is_in->water_can_be_pushed_upwards(this);
}

int WaterParticle::water_to_the_left() {
        return world_is_in->amount_of_water_to_the_left_of(this);
}

int WaterParticle::water_to_the_right() {
        return world_is_in->amount_of_water_to_the_right_of(this);
}

int WaterParticle::water_from_to_the_left() {
        return 1 + world_is_in->amount_of_water_to_the_left_of(this);
}

int WaterParticle::water_from_to_the_right() {
        return 1 + world_is_in->amount_of_water_to_the_right_of(this);
}

void WaterParticle::accept(WorldVisitor& visitor) const {
        visitor.visit_water_particle(*this);
}

void WaterParticle::step() {
}

void WaterParticle::particle_is_pushing_onto(Particle* particle){
        particle->push_onto_water(this);
}

void WaterParticle::push_onto_dirt(DirtParticle* dirt_particle){
        world_is_in->water_pushing_onto_solid(this, dirt_particle);
        world_is_in->turn_dirt_into_mud(dirt_particle);
}

void WaterParticle::push_onto_void(VoidParticle* void_particle){
        Particle::push_onto_void(void_particle);
}

void WaterParticle::push_onto_water(WaterParticle* water_particle){
        world_is_in->water_pushing_onto_water(this, water_particle);
}

void WaterParticle::push_onto_mud(MudParticle* mud_particle){
        world_is_in->water_pushing_onto_solid(this, mud_particle);
}

void WaterParticle::push_onto_grass(GrassParticle* grass_particle){
        world_is_in->water_pushing_onto_solid(this, grass_particle);
}

void WaterParticle::push_onto_stone(StoneParticle* stone_particle){
        world_is_in->water_pushing_onto_solid(this, stone_particle);
}

void WaterParticle::push_onto_wood(WoodParticle* wood_particle){
        world_is_in->water_pushing_onto_wood(this, wood_particle);
}

void WaterParticle::push_onto_fire(FireParticle* fire_particle){
        world_is_in->water_pushing_onto_fire(this, fire_particle);
}

void WaterParticle::dirt_pushing_to_the_left(DirtParticle* dirt_particle){
}

void WaterParticle::dirt_pushing_to_the_right(DirtParticle* dirt_particle){
}

void WaterParticle::mud_pushing_to_the_left(MudParticle* mud_particle){
}

void WaterParticle::mud_pushing_to_the_right(MudParticle* mud_particle){
}

void WaterParticle::water_pushing_to_the_left(WaterParticle* water_particle){
}

void WaterParticle::water_pushing_to_the_right(WaterParticle* water_particle){
}

void WaterParticle::grass_trying_to_spread_onto(){}

void WaterParticle::burn(){}

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

bool MudParticle::isStone() const {
        return false;
}

bool MudParticle::isWood() const {
        return false;
}


bool MudParticle::isFire() const {
        return false;
}

bool MudParticle::dirt_can_push_through() const {
  return false;
}

bool MudParticle::can_pass_through_it(Particle* blocking_particle) {
        return blocking_particle->mud_can_pass_through_it();
}

bool MudParticle::can_be_pushed_into_by_water() const {
        return false;
}

bool MudParticle::water_can_push_it_to_the_left() {
        return false;
}

bool MudParticle::water_can_push_it_to_the_right() {
        return false;
}

bool MudParticle::water_can_push_it_upwards() {
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
}

void MudParticle::particle_is_pushing_onto(Particle* particle){
        particle->push_onto_mud(this);
}

void MudParticle::push_onto_dirt(DirtParticle* dirt_particle){
        Particle::push_onto_dirt(dirt_particle);
}

void MudParticle::push_onto_void(VoidParticle* void_particle){
        Particle::push_onto_void(void_particle);
}

void MudParticle::push_onto_water(WaterParticle* water_particle){
        world_is_in->solid_pushing_onto_water(this, water_particle);
}

void MudParticle::push_onto_mud(MudParticle* mud_particle){
        world_is_in->mud_particle_pushing_onto_mud(this, mud_particle);
}

void MudParticle::push_onto_fire(FireParticle* fire_particle){
        Particle::push_onto_fire(fire_particle);
}

void MudParticle::dirt_pushing_to_the_left(DirtParticle* dirt_particle){
}

void MudParticle::dirt_pushing_to_the_right(DirtParticle* dirt_particle){
}

void MudParticle::mud_pushing_to_the_left(MudParticle* mud_particle){
}

void MudParticle::mud_pushing_to_the_right(MudParticle* mud_particle){
}

void MudParticle::water_pushing_to_the_left(WaterParticle* water_particle){
}

void MudParticle::water_pushing_to_the_right(WaterParticle* water_particle){
}

void MudParticle::grass_trying_to_spread_onto(){
}

void MudParticle::burn(){
}

StoneParticle::StoneParticle(World* world) : Particle(world) {
}

bool StoneParticle::isDirt() const {
        return false;
}

bool StoneParticle::isGrass() const {
        return false;
}

bool StoneParticle::isVoid() const {
        return false;
}

bool StoneParticle::isWater() const {
        return false;
}

bool StoneParticle::isMud() const {
        return false;
}

bool StoneParticle::isStone() const {
        return true;
}

bool StoneParticle::isWood() const {
        return false;
}

bool StoneParticle::isFire() const {
        return false;
}

bool StoneParticle::isDam() {
        return true;
}

bool StoneParticle::isDamUpwards() {
        return true;
}

bool StoneParticle::isDamDownwards() {
        return true;
}

bool StoneParticle::dirt_can_push_through() const {
        return false;
}

bool StoneParticle::can_pass_through_it(Particle* blocking_particle) {
        return blocking_particle->stone_can_pass_through_it();
}

bool StoneParticle::can_be_pushed_into_by_water() const {
        return false;
}

bool StoneParticle::water_can_push_it_to_the_left() {
        return false;
}

bool StoneParticle::water_can_push_it_to_the_right() {
        return false;
}

bool StoneParticle::water_can_push_it_upwards() {
        return false;
}

int StoneParticle::water_to_the_left() {
        return 0;
}

int StoneParticle::water_to_the_right() {
        return 0;
}

int StoneParticle::water_from_to_the_left() {
        return 0;
}

int StoneParticle::water_from_to_the_right() {
        return 0;
}

void StoneParticle::accept(WorldVisitor& visitor) const {
        visitor.visit_stone_particle(*this);
}

void StoneParticle::step() {
        world_is_in->support_stone(this);
}

void StoneParticle::particle_is_pushing_onto(Particle* particle){
        particle->push_onto_stone(this);
}

void StoneParticle::push_onto_dirt(DirtParticle* dirt_particle){
        Particle::push_onto_dirt(dirt_particle);
}

void StoneParticle::push_onto_void(VoidParticle* void_particle){
        Particle::push_onto_void(void_particle);
}

void StoneParticle::push_onto_water(WaterParticle* water_particle){
        world_is_in->solid_pushing_onto_water(this, water_particle);
}

void StoneParticle::push_onto_mud(MudParticle* mud_particle){
        Particle::push_onto_mud(mud_particle);
}

void StoneParticle::push_onto_fire(FireParticle* fire_particle){
        Particle::push_onto_fire(fire_particle);
}

void StoneParticle::dirt_pushing_to_the_left(DirtParticle* dirt_particle){
}

void StoneParticle::dirt_pushing_to_the_right(DirtParticle* dirt_particle){
}

void StoneParticle::mud_pushing_to_the_left(MudParticle* mud_particle){
}

void StoneParticle::mud_pushing_to_the_right(MudParticle* mud_particle){
}

void StoneParticle::water_pushing_to_the_left(WaterParticle* water_particle){
}

void StoneParticle::water_pushing_to_the_right(WaterParticle* water_particle){
}

void StoneParticle::grass_trying_to_spread_onto(){
}

void StoneParticle::burn(){
}

void StoneParticle::support(StoneParticle* stone_particle) {
        world_is_in->reset_speed(stone_particle);
}

WoodParticle::WoodParticle(World* world) : Particle(world) {
}

bool WoodParticle::isDirt() const {
        return false;
}

bool WoodParticle::isGrass() const {
        return false;
}

bool WoodParticle::isVoid() const {
        return false;
}

bool WoodParticle::isWater() const {
        return false;
}

bool WoodParticle::isMud() const {
        return false;
}

bool WoodParticle::isStone() const {
        return false;
}

bool WoodParticle::isWood() const {
        return true;
}

bool WoodParticle::isFire() const {
        return false;
}

bool WoodParticle::isDam() {
        return world_is_in->isDamStructureUpwards(this) &&
                world_is_in->isDamStructureDownwards(this);
}

bool WoodParticle::isDamUpwards() {
        return world_is_in->isDamStructureUpwards(this);
}

bool WoodParticle::isDamDownwards() {
        return world_is_in->isDamStructureDownwards(this);
}

bool WoodParticle::dirt_can_push_through() const {
        return false;
}

bool WoodParticle::can_pass_through_it(Particle* blocking_particle) {
        return blocking_particle->wood_can_pass_through_it();
}

bool WoodParticle::can_float_on_water() {
        return true;
}

bool WoodParticle::can_be_pushed_into_by_water() const {
        return false;
}

bool WoodParticle::water_can_push_it_to_the_left() {
        if (isDam()) {
                return false;
        }

        return world_is_in->can_be_moved_to_the_left(this);
}

bool WoodParticle::water_can_push_it_to_the_right() {
        if (isDam()) {
                return false;
        }

        return world_is_in->can_be_moved_to_the_right(this);
}

bool WoodParticle::water_can_push_it_upwards() {
        return true;
}

int WoodParticle::water_to_the_left() {
        return 0;
}

int WoodParticle::water_to_the_right() {
        return 0;
}

int WoodParticle::water_from_to_the_left() {
        return 0;
}

int WoodParticle::water_from_to_the_right() {
        return 0;
}

void WoodParticle::accept(WorldVisitor& visitor) const {
        visitor.visit_wood_particle(*this);
}

void WoodParticle::step() {
}

void WoodParticle::particle_is_pushing_onto(Particle* particle){
        particle->push_onto_wood(this);
}

void WoodParticle::push_onto_dirt(DirtParticle* dirt_particle){
        Particle::push_onto_dirt(dirt_particle);
}

void WoodParticle::push_onto_void(VoidParticle* void_particle){
        Particle::push_onto_void(void_particle);
}

void WoodParticle::push_onto_water(WaterParticle* water_particle){
        world_is_in->wood_pushing_onto_water(this, water_particle);
}

void WoodParticle::push_onto_mud(MudParticle* mud_particle){
        Particle::push_onto_mud(mud_particle);
}

void WoodParticle::dirt_pushing_to_the_left(DirtParticle* dirt_particle){
}

void WoodParticle::dirt_pushing_to_the_right(DirtParticle* dirt_particle){
}

void WoodParticle::mud_pushing_to_the_left(MudParticle* mud_particle){
}

void WoodParticle::mud_pushing_to_the_right(MudParticle* mud_particle){
}

void WoodParticle::water_pushing_to_the_left(WaterParticle* water_particle){
}

void WoodParticle::water_pushing_to_the_right(WaterParticle* water_particle){
}

void WoodParticle::grass_trying_to_spread_onto(){
}

void WoodParticle::burn(){
        steps_until_burning_out--;
        if(steps_until_burning_out <= 0){
                world_is_in->fire_spreading_onto(this);
        }
}

FireParticle::FireParticle(World* world) : Particle(world) {
}

bool FireParticle::isDirt() const {
        return false;
}

bool FireParticle::isGrass() const {
        return false;
}

bool FireParticle::isVoid() const {
        return false;
}

bool FireParticle::isWater() const {
        return false;
}

bool FireParticle::isMud() const {
        return false;
}

bool FireParticle::isStone() const {
        return false;
}

bool FireParticle::isWood() const {
        return false;
}

bool FireParticle::isFire() const {
        return true;
}

bool FireParticle::dirt_can_push_through() const {
        return true;
}

bool FireParticle::can_pass_through_it(Particle* blocking_particle) {
        return blocking_particle->fire_can_pass_through_it();
}

bool FireParticle::dirt_can_pass_through_it() {
        return true;
}

bool FireParticle::grass_can_pass_through_it() {
        return false;
}

bool FireParticle::water_can_pass_through_it() {
        return false;
}

bool FireParticle::mud_can_pass_through_it() {
        return true;
}

bool FireParticle::stone_can_pass_through_it() {
        return true;
}

bool FireParticle::wood_can_pass_through_it() {
        return false;
}

bool FireParticle::fire_can_pass_through_it() {
        return false;
}

bool FireParticle::can_be_pushed_into_by_water() const {
        return true;
}

bool FireParticle::water_can_push_it_to_the_left() {
        return false;
}

bool FireParticle::water_can_push_it_to_the_right() {
        return false;
}

int FireParticle::water_to_the_left() {
        return 0;
}

int FireParticle::water_to_the_right() {
        return 0;
}

int FireParticle::water_from_to_the_left() {
        return 0;
}

int FireParticle::water_from_to_the_right() {
        return 0;
}

void FireParticle::accept(WorldVisitor& visitor) const {
        visitor.visit_fire_particle(*this);
}

void FireParticle::step() {

        steps_until_being_put_off--;
        if(steps_until_being_put_off <= 0){
                world_is_in->fire_died(this);
                return;
        }

        world_is_in->fire_trying_to_spread(this);
}

void FireParticle::particle_is_pushing_onto(Particle* particle){
        particle->push_onto_fire(this);
}

void FireParticle::push_onto_dirt(DirtParticle* dirt_particle){
        Particle::push_onto_dirt(dirt_particle);
}

void FireParticle::push_onto_void(VoidParticle* void_particle){
        Particle::push_onto_void(void_particle);
}

void FireParticle::push_onto_water(WaterParticle* water_particle){
        world_is_in->fire_pushing_onto_water(this, water_particle);
}

void FireParticle::push_onto_mud(MudParticle* mud_particle){
        steps_until_being_put_off -= 10;
        Particle::push_onto_mud(mud_particle);
}

void FireParticle::push_onto_grass(GrassParticle* grass_particle){
        Particle::push_onto_grass(grass_particle);
}

void FireParticle::dirt_pushing_to_the_left(DirtParticle* dirt_particle){
}

void FireParticle::dirt_pushing_to_the_right(DirtParticle* dirt_particle){
        world_is_in->dirt_falling_to_the_right_onto_fire(dirt_particle, this);
}

void FireParticle::mud_pushing_to_the_left(MudParticle* mud_particle){
        world_is_in->mud_falling_to_the_left_onto_fire(mud_particle, this);
}

void FireParticle::mud_pushing_to_the_right(MudParticle* mud_particle){
        world_is_in->mud_falling_to_the_right_onto_fire(mud_particle, this);
}

void FireParticle::water_pushing_to_the_left(WaterParticle* water_particle){
}

void FireParticle::water_pushing_to_the_right(WaterParticle* water_particle){
}

void FireParticle::grass_trying_to_spread_onto(){
}

void FireParticle::burn(){
}
