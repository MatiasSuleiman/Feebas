#pragma once

#include <sstream>
#include <string>
#include <vector>

#include "world_cell_change.hpp"
#include "world_visitor.hpp"

class WorldChangesToJSONVisitor : public WorldVisitor {
 public:
        explicit WorldChangesToJSONVisitor(int revision);
        void visit_world(int width, int height) override;
        void finish_visiting_world() override;
        void visit_dirt_particle(const DirtParticle& particle) override;
        void visit_grass_particle(const GrassParticle& particle) override;
        void visit_void_particle(const VoidParticle& particle) override;
        void visit_water_particle(const WaterParticle& particle) override;
        void visit_mud_particle(const MudParticle& particle) override;
        void visit_stone_particle(const StoneParticle& particle) override;
        void visit_wood_particle(const WoodParticle& particle) override;
        void visit_fire_particle(const FireParticle& particle) override;
        void visit_TNT_particle(const TNTParticle& particle) override;
        std::string json_for(const std::vector<WorldCellChange>& changes);

 private:
        void append_change(const WorldCellChange& change);
        void set_particle_type(const std::string& particle_type);

        int world_revision;
        std::ostringstream output;
        std::string current_particle_type;
        bool first_change = true;
};
