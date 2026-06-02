#pragma once

class DirtParticle;
class GrassParticle;
class VoidParticle;
class WaterParticle;
class MudParticle;
class StoneParticle;
class WoodParticle;

class WorldVisitor {
 public:
        virtual ~WorldVisitor() = default;
        virtual void visit_world(int width, int height) = 0;
        virtual void finish_visiting_world() = 0;
        virtual void visit_dirt_particle(const DirtParticle& particle) = 0;
        virtual void visit_grass_particle(const GrassParticle& particle) = 0;
        virtual void visit_void_particle(const VoidParticle& particle) = 0;
        virtual void visit_water_particle(const WaterParticle& particle) = 0;
        virtual void visit_mud_particle(const MudParticle& particle) = 0;
        virtual void visit_stone_particle(const StoneParticle& particle) = 0;
        virtual void visit_wood_particle(const WoodParticle& particle) = 0;
};
