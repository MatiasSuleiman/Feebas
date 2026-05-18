#include "particles.hpp"

Particle::~Particle() {
}

DirtParticle::DirtParticle() {
}

bool DirtParticle::isDirt() const {
  return true;
}

bool DirtParticle::isVoid() const {
  return false;
}

void DirtParticle::step(int x, int y) const{
        
}

VoidParticle::VoidParticle() {
}

bool VoidParticle::isDirt() const {
  return false;
}

bool VoidParticle::isVoid() const {
  return true;
}

