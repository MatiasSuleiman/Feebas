#include "world_changes_to_json_visitor.hpp"

#include "particles.hpp"

WorldChangesToJSONVisitor::WorldChangesToJSONVisitor(int revision)
        : world_revision(revision) {
}

void WorldChangesToJSONVisitor::visit_world(int, int) {
}

void WorldChangesToJSONVisitor::finish_visiting_world() {
}

void WorldChangesToJSONVisitor::visit_dirt_particle(const DirtParticle&) {
        set_particle_type("DirtParticle");
}

void WorldChangesToJSONVisitor::visit_grass_particle(const GrassParticle&) {
        set_particle_type("GrassParticle");
}

void WorldChangesToJSONVisitor::visit_void_particle(const VoidParticle&) {
        set_particle_type("VoidParticle");
}

void WorldChangesToJSONVisitor::visit_water_particle(const WaterParticle&) {
        set_particle_type("WaterParticle");
}

void WorldChangesToJSONVisitor::visit_mud_particle(const MudParticle&) {
        set_particle_type("MudParticle");
}

void WorldChangesToJSONVisitor::visit_stone_particle(const StoneParticle&) {
        set_particle_type("StoneParticle");
}

void WorldChangesToJSONVisitor::visit_wood_particle(const WoodParticle&) {
        set_particle_type("WoodParticle");
}

void WorldChangesToJSONVisitor::visit_fire_particle(const FireParticle&) {
        set_particle_type("FireParticle");
}

std::string WorldChangesToJSONVisitor::json_for(const std::vector<WorldCellChange>& changes) {
        output << "{\"revision\":" << world_revision << ",\"changes\":[";

        for (const WorldCellChange& change : changes) {
                append_change(change);
        }

        output << "]}";
        return output.str();
}

void WorldChangesToJSONVisitor::append_change(const WorldCellChange& change) {
        current_particle_type.clear();
        change.particle->accept(*this);

        if (!first_change) {
                output << ',';
        }

        first_change = false;
        output << "{\"x\":" << change.x
                << ",\"y\":" << change.y
                << ",\"particle\":\"" << current_particle_type << "\"}";
}

void WorldChangesToJSONVisitor::set_particle_type(const std::string& particle_type) {
        current_particle_type = particle_type;
}
