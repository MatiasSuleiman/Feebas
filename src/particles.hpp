#pragma once

class World;
class DirtParticle;
class VoidParticle;
class GrassParticle;
class WaterParticle;
class MudParticle;
class WorldVisitor;

class Particle {
        public:

                explicit Particle(World* world);
                virtual ~Particle() = 0;
                virtual bool isDirt() const = 0;
                virtual bool isGrass() const = 0;
                virtual bool isVoid() const = 0;
                virtual bool isWater() const = 0;
                virtual bool isMud() const = 0;
                virtual bool dirt_can_fall_through() const = 0;
                virtual bool can_be_pushed_into_by_water() const = 0;
                virtual bool can_be_pushed_to_the_left() = 0;
                virtual bool can_be_pushed_to_the_right() = 0;
                virtual int water_to_the_left() = 0;
                virtual int water_to_the_right() = 0;
                virtual int water_from_to_the_left() = 0;
                virtual int water_from_to_the_right() = 0;
                virtual void accept(WorldVisitor& visitor) const = 0;
                virtual void step() = 0;
                virtual void particle_is_falling_onto(Particle* particle) = 0;
                virtual void fall_onto_dirt(DirtParticle* dirt_particle) = 0;
                virtual void fall_onto_void(VoidParticle* void_particle) = 0;
                virtual void fall_onto_water(WaterParticle* water_particle) = 0;
                virtual void fall_onto_mud(MudParticle* mud_particle) = 0;
                virtual void dirt_falling_to_the_left(DirtParticle* dirt_particle) = 0;
                virtual void dirt_falling_to_the_right(DirtParticle* dirt_particle) = 0;
                virtual void mud_falling_to_the_left(MudParticle* mud_particle) = 0;
                virtual void mud_falling_to_the_right(MudParticle* mud_particle) = 0;
                virtual void water_falling_to_the_left(WaterParticle* water_particle) = 0;
                virtual void water_falling_to_the_right(WaterParticle* water_particle) = 0;
                virtual void grass_trying_to_spread_onto() = 0;

        protected:
                World* world_is_in;
};

class DirtParticle : public Particle {
        public:

                DirtParticle(World* world);
                bool isDirt() const override;
                bool isGrass() const override;
                bool isVoid() const override;
                bool isWater() const override;
                bool isMud() const override;
                bool dirt_can_fall_through() const override;
                bool can_be_pushed_into_by_water() const override;
                bool can_be_pushed_to_the_left() override;
                bool can_be_pushed_to_the_right() override;
                int water_to_the_left() override;
                int water_to_the_right() override;
                int water_from_to_the_left() override;
                int water_from_to_the_right() override;
                void accept(WorldVisitor& visitor) const override;
                void step() override;
                void particle_is_falling_onto(Particle* particle) override;
                void fall_onto_dirt(DirtParticle* dirt_particle) override;
                void fall_onto_void(VoidParticle* void_particle) override;
                void fall_onto_water(WaterParticle* water_particle) override;
                void fall_onto_mud(MudParticle* mud_particle) override;
                void dirt_falling_to_the_left(DirtParticle* dirt_particle) override;
                void dirt_falling_to_the_right(DirtParticle* dirt_particle) override;
                void mud_falling_to_the_left(MudParticle* mud_particle) override;
                void mud_falling_to_the_right(MudParticle* mud_particle) override;
                void water_falling_to_the_left(WaterParticle* water_particle) override;
                void water_falling_to_the_right(WaterParticle* water_particle) override;
                void grass_trying_to_spread_onto() override;

        private:
                int steps_until_being_grass = 50;

};

class VoidParticle : public Particle {
        public:

                VoidParticle(World* world);
                bool isDirt() const override;
                bool isGrass() const override;
                bool isVoid() const override;
                bool isWater() const override;
                bool isMud() const override;
                bool dirt_can_fall_through() const override;
                bool can_be_pushed_into_by_water() const override;
                bool can_be_pushed_to_the_left() override;
                bool can_be_pushed_to_the_right() override;
                int water_to_the_left() override;
                int water_to_the_right() override;
                int water_from_to_the_left() override;
                int water_from_to_the_right() override;
                void accept(WorldVisitor& visitor) const override;
                void step() override;
                void particle_is_falling_onto(Particle* particle) override;
                void fall_onto_dirt(DirtParticle* dirt_particle) override;
                void fall_onto_void(VoidParticle* void_particle) override;
                void fall_onto_water(WaterParticle* water_particle) override;
                void fall_onto_mud(MudParticle* mud_particle) override;
                void dirt_falling_to_the_left(DirtParticle* dirt_particle) override;
                void dirt_falling_to_the_right(DirtParticle* dirt_particle) override;
                void mud_falling_to_the_left(MudParticle* mud_particle) override;
                void mud_falling_to_the_right(MudParticle* mud_particle) override;
                void water_falling_to_the_left(WaterParticle* water_particle) override;
                void water_falling_to_the_right(WaterParticle* water_particle) override;
                void grass_trying_to_spread_onto() override;

};

class GrassParticle : public Particle {
        public:

                GrassParticle(World* world);
                bool isDirt() const override;
                bool isGrass() const override;
                bool isVoid() const override;
                bool isWater() const override;
                bool isMud() const override;
                bool dirt_can_fall_through() const override;
                bool can_be_pushed_into_by_water() const override;
                bool can_be_pushed_to_the_left() override;
                bool can_be_pushed_to_the_right() override;
                int water_to_the_left() override;
                int water_to_the_right() override;
                int water_from_to_the_left() override;
                int water_from_to_the_right() override;
                void accept(WorldVisitor& visitor) const override;
                void step() override;
                void particle_is_falling_onto(Particle* particle) override;
                void fall_onto_dirt(DirtParticle* dirt_particle) override;
                void fall_onto_void(VoidParticle* void_particle) override;
                void fall_onto_water(WaterParticle* water_particle) override;
                void fall_onto_mud(MudParticle* mud_particle) override;
                void dirt_falling_to_the_left(DirtParticle* dirt_particle) override;
                void dirt_falling_to_the_right(DirtParticle* dirt_particle) override;
                void mud_falling_to_the_left(MudParticle* mud_particle) override;
                void mud_falling_to_the_right(MudParticle* mud_particle) override;
                void water_falling_to_the_left(WaterParticle* water_particle) override;
                void water_falling_to_the_right(WaterParticle* water_particle) override;
                void grass_trying_to_spread_onto() override;

};

class WaterParticle : public Particle {
        public:

                WaterParticle(World* world);
                bool isDirt() const override;
                bool isGrass() const override;
                bool isVoid() const override;
                bool isWater() const override;
                bool isMud() const override;
                bool dirt_can_fall_through() const override;
                bool can_be_pushed_into_by_water() const override;
                bool can_be_pushed_to_the_left() override;
                bool can_be_pushed_to_the_right() override;
                int water_to_the_left() override;
                int water_to_the_right() override;
                int water_from_to_the_left() override;
                int water_from_to_the_right() override;
                void accept(WorldVisitor& visitor) const override;
                void step() override;
                void particle_is_falling_onto(Particle* particle) override;
                void fall_onto_dirt(DirtParticle* dirt_particle) override;
                void fall_onto_void(VoidParticle* void_particle) override;
                void fall_onto_water(WaterParticle* water_particle) override;
                void fall_onto_mud(MudParticle* mud_particle) override;
                void dirt_falling_to_the_left(DirtParticle* dirt_particle) override;
                void dirt_falling_to_the_right(DirtParticle* dirt_particle) override;
                void mud_falling_to_the_left(MudParticle* mud_particle) override;
                void mud_falling_to_the_right(MudParticle* mud_particle) override;
                void water_falling_to_the_left(WaterParticle* water_particle) override;
                void water_falling_to_the_right(WaterParticle* water_particle) override;
                void grass_trying_to_spread_onto() override;

};

class MudParticle : public Particle {
        public:

                MudParticle(World* world);
                bool isDirt() const override;
                bool isGrass() const override;
                bool isVoid() const override;
                bool isWater() const override;
                bool isMud() const override;
                bool dirt_can_fall_through() const override;
                bool can_be_pushed_into_by_water() const override;
                bool can_be_pushed_to_the_left() override;
                bool can_be_pushed_to_the_right() override;
                int water_to_the_left() override;
                int water_to_the_right() override;
                int water_from_to_the_left() override;
                int water_from_to_the_right() override;
                void accept(WorldVisitor& visitor) const override;
                void step() override;
                void particle_is_falling_onto(Particle* particle) override;
                void fall_onto_dirt(DirtParticle* dirt_particle) override;
                void fall_onto_void(VoidParticle* void_particle) override;
                void fall_onto_water(WaterParticle* water_particle) override;
                void fall_onto_mud(MudParticle* mud_particle) override;
                void dirt_falling_to_the_left(DirtParticle* dirt_particle) override;
                void dirt_falling_to_the_right(DirtParticle* dirt_particle) override;
                void mud_falling_to_the_left(MudParticle* mud_particle) override;
                void mud_falling_to_the_right(MudParticle* mud_particle) override;
                void water_falling_to_the_left(WaterParticle* water_particle) override;
                void water_falling_to_the_right(WaterParticle* water_particle) override;
                void grass_trying_to_spread_onto() override;

};
