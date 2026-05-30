#pragma once

#include <sstream>
#include <string>

#include "world_visitor.hpp"

class WorldToJSONVisitor : public WorldVisitor {
 public:
        void visit_world(int width, int height) override;
        void finish_visiting_world() override;
        void visit_dirt_particle(const DirtParticle& particle) override;
        void visit_grass_particle(const GrassParticle& particle) override;
        void visit_void_particle(const VoidParticle& particle) override;
        void visit_water_particle(const WaterParticle& particle) override;
        void visit_mud_particle(const MudParticle& particle) override;
        std::string json() const;

 private:
        void append_particle_type(const std::string& particle_type);

        std::ostringstream output;
        bool first_cell = true;
};
