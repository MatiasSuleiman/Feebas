#pragma once

class World;
class DirtParticle;
class VoidParticle;

class Particle {
        public:

                explicit Particle(World* world);
                virtual ~Particle() = 0;
                virtual bool isDirt() const = 0;
                virtual bool isVoid() const = 0;
                virtual void step() = 0;
                virtual void particle_is_falling_onto(Particle* particle) = 0;
                virtual void fall_onto_dirt(DirtParticle* dirt_particle) = 0;
                virtual void fall_onto_void(VoidParticle* void_particle) = 0;

        protected:
                World* world_is_in;
};

class DirtParticle : public Particle {
        public:

                DirtParticle(World* world);
                bool isDirt() const override;
                bool isVoid() const override;
                void step() override;
                void particle_is_falling_onto(Particle* particle) override;
                void fall_onto_dirt(DirtParticle* dirt_particle) override;
                void fall_onto_void(VoidParticle* void_particle) override;

};

class VoidParticle : public Particle {
        public:

                VoidParticle(World* world);
                bool isDirt() const override;
                bool isVoid() const override;
                void step() override;
                void particle_is_falling_onto(Particle* particle) override;
                void fall_onto_dirt(DirtParticle* dirt_particle) override;
                void fall_onto_void(VoidParticle* void_particle) override;

};
