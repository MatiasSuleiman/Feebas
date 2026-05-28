#include "world_to_json_visitor.hpp"

void WorldToJSONVisitor::visit_world(int width, int height) {
  output << "{\"width\":" << width << ",\"height\":" << height << ",\"cells\":[";
}

void WorldToJSONVisitor::finish_visiting_world() {
  output << "]}";
}

void WorldToJSONVisitor::visit_dirt_particle(const DirtParticle&) {
  append_particle_type("DirtParticle");
}

void WorldToJSONVisitor::visit_grass_particle(const GrassParticle&) {
  append_particle_type("GrassParticle");
}

void WorldToJSONVisitor::visit_void_particle(const VoidParticle&) {
  append_particle_type("VoidParticle");
}

void WorldToJSONVisitor::visit_water_particle(const WaterParticle&) {
  append_particle_type("WaterParticle");
}

std::string WorldToJSONVisitor::json() const {
  return output.str();
}

void WorldToJSONVisitor::append_particle_type(const std::string& particle_type) {
  if (!first_cell) {
    output << ',';
  }

  first_cell = false;
  output << '"' << particle_type << '"';
}
