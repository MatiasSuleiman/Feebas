#pragma once

#include <vector>
class World;
class DirtParticle;
class VoidParticle;
class GrassParticle;
class WaterParticle;
class MudParticle;
class StoneParticle;
class WoodParticle;
class FireParticle;
class TNTParticle;

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
                virtual bool isStone() const = 0;
                virtual bool isWood() const = 0;
                virtual bool isFire() const = 0;
                virtual bool isTNT() const = 0;
                virtual bool isDam();
                virtual bool isDamUpwards();
                virtual bool isDamDownwards();
                virtual bool can_make_waves();
                virtual bool can_overflow();
                virtual bool can_float_on_water();
                virtual bool dirt_can_push_through() const = 0;
                virtual bool can_pass_through_it(Particle* blocking_particle) = 0;
                virtual bool dirt_can_pass_through_it();
                virtual bool grass_can_pass_through_it();
                virtual bool water_can_pass_through_it();
                virtual bool mud_can_pass_through_it();
                virtual bool stone_can_pass_through_it();
                virtual bool wood_can_pass_through_it();
                virtual bool fire_can_pass_through_it();
                virtual bool tnt_can_pass_through_it();
                virtual bool can_be_pushed_into_by_water() const = 0;
                virtual bool water_can_push_it_to_the_left() = 0;
                virtual bool water_can_push_it_to_the_right() = 0;
                virtual bool water_can_push_it_upwards();
                virtual int water_to_the_left() = 0;
                virtual int water_to_the_right() = 0;
                virtual int water_from_to_the_left() = 0;
                virtual int water_from_to_the_right() = 0;
                virtual void accept(WorldVisitor& visitor) const = 0;
                virtual void step() = 0;
                virtual void particle_is_pushing_onto(Particle* particle) = 0;
                virtual void push_onto_dirt(DirtParticle* dirt_particle);
                virtual void push_onto_void(VoidParticle* void_particle);
                virtual void push_onto_water(WaterParticle* water_particle);
                virtual void push_onto_mud(MudParticle* mud_particle);
                virtual void push_onto_grass(GrassParticle* grass_particle);
                virtual void push_onto_stone(StoneParticle* stone_particle);
                virtual void push_onto_wood(WoodParticle* wood_particle);
                virtual void push_onto_fire(FireParticle* fire_particle);
                virtual void dirt_pushing_to_the_left(DirtParticle* dirt_particle) = 0;
                virtual void dirt_pushing_to_the_right(DirtParticle* dirt_particle) = 0;
                virtual void mud_pushing_to_the_left(MudParticle* mud_particle) = 0;
                virtual void mud_pushing_to_the_right(MudParticle* mud_particle) = 0;
                virtual void water_pushing_to_the_left(WaterParticle* water_particle) = 0;
                virtual void water_pushing_to_the_right(WaterParticle* water_particle) = 0;
                virtual void grass_trying_to_spread_onto() = 0;
                virtual void burn() = 0;
                virtual void support(StoneParticle* stone_particle);

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
                bool isStone() const override;
                bool isWood() const override;
                bool isFire() const override;
                bool isTNT() const override;
                bool dirt_can_push_through() const override;
                bool can_pass_through_it(Particle* blocking_particle) override;
                bool can_be_pushed_into_by_water() const override;
                bool water_can_push_it_to_the_left() override;
                bool water_can_push_it_to_the_right() override;
                bool water_can_push_it_upwards() override;
                int water_to_the_left() override;
                int water_to_the_right() override;
                int water_from_to_the_left() override;
                int water_from_to_the_right() override;
                void accept(WorldVisitor& visitor) const override;
                void step() override;
                void particle_is_pushing_onto(Particle* particle) override;
                void push_onto_dirt(DirtParticle* dirt_particle) override;
                void push_onto_void(VoidParticle* void_particle) override;
                void push_onto_water(WaterParticle* water_particle) override;
                void push_onto_mud(MudParticle* mud_particle) override;
                void push_onto_fire(FireParticle* fire_particle) override;
                void dirt_pushing_to_the_left(DirtParticle* dirt_particle) override;
                void dirt_pushing_to_the_right(DirtParticle* dirt_particle) override;
                void mud_pushing_to_the_left(MudParticle* mud_particle) override;
                void mud_pushing_to_the_right(MudParticle* mud_particle) override;
                void water_pushing_to_the_left(WaterParticle* water_particle) override;
                void water_pushing_to_the_right(WaterParticle* water_particle) override;
                void grass_trying_to_spread_onto() override;
                void burn() override;

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
                bool isStone() const override;
                bool isWood() const override;
                bool isFire() const override;
                bool isTNT() const override;
                bool dirt_can_push_through() const override;
                bool can_pass_through_it(Particle* blocking_particle) override;
                bool dirt_can_pass_through_it() override;
                bool grass_can_pass_through_it() override;
                bool water_can_pass_through_it() override;
                bool mud_can_pass_through_it() override;
                bool stone_can_pass_through_it() override;
                bool wood_can_pass_through_it() override;
                bool fire_can_pass_through_it() override;
                bool tnt_can_pass_through_it() override;
                bool can_be_pushed_into_by_water() const override;
                bool water_can_push_it_to_the_left() override;
                bool water_can_push_it_to_the_right() override;
                int water_to_the_left() override;
                int water_to_the_right() override;
                int water_from_to_the_left() override;
                int water_from_to_the_right() override;
                void accept(WorldVisitor& visitor) const override;
                void step() override;
                void particle_is_pushing_onto(Particle* particle) override;
                void push_onto_dirt(DirtParticle* dirt_particle) override;
                void push_onto_void(VoidParticle* void_particle) override;
                void push_onto_water(WaterParticle* water_particle) override;
                void push_onto_mud(MudParticle* mud_particle) override;
                void dirt_pushing_to_the_left(DirtParticle* dirt_particle) override;
                void dirt_pushing_to_the_right(DirtParticle* dirt_particle) override;
                void mud_pushing_to_the_left(MudParticle* mud_particle) override;
                void mud_pushing_to_the_right(MudParticle* mud_particle) override;
                void water_pushing_to_the_left(WaterParticle* water_particle) override;
                void water_pushing_to_the_right(WaterParticle* water_particle) override;
                void grass_trying_to_spread_onto() override;
                void burn() override;

};

class GrassParticle : public Particle {
        public:

                GrassParticle(World* world);
                bool isDirt() const override;
                bool isGrass() const override;
                bool isVoid() const override;
                bool isWater() const override;
                bool isMud() const override;
                bool isStone() const override;
                bool isWood() const override;
                bool isFire() const override;
                bool isTNT() const override;
                bool dirt_can_push_through() const override;
                bool can_pass_through_it(Particle* blocking_particle) override;
                bool can_float_on_water() override;
                bool fire_can_pass_through_it() override;
                bool can_be_pushed_into_by_water() const override;
                bool water_can_push_it_to_the_left() override;
                bool water_can_push_it_to_the_right() override;
                bool water_can_push_it_upwards() override;
                int water_to_the_left() override;
                int water_to_the_right() override;
                int water_from_to_the_left() override;
                int water_from_to_the_right() override;
                void accept(WorldVisitor& visitor) const override;
                void step() override;
                void particle_is_pushing_onto(Particle* particle) override;
                void push_onto_dirt(DirtParticle* dirt_particle) override;
                void push_onto_void(VoidParticle* void_particle) override;
                void push_onto_water(WaterParticle* water_particle) override;
                void push_onto_mud(MudParticle* mud_particle) override;
                void push_onto_fire(FireParticle* fire_particle) override;
                void dirt_pushing_to_the_left(DirtParticle* dirt_particle) override;
                void dirt_pushing_to_the_right(DirtParticle* dirt_particle) override;
                void mud_pushing_to_the_left(MudParticle* mud_particle) override;
                void mud_pushing_to_the_right(MudParticle* mud_particle) override;
                void water_pushing_to_the_left(WaterParticle* water_particle) override;
                void water_pushing_to_the_right(WaterParticle* water_particle) override;
                void grass_trying_to_spread_onto() override;
                void burn() override;

        private:
                int steps_until_burning_out = 5;

};

class WaterParticle : public Particle {
        public:

                WaterParticle(World* world);
                bool isDirt() const override;
                bool isGrass() const override;
                bool isVoid() const override;
                bool isWater() const override;
                bool isMud() const override;
                bool isStone() const override;
                bool isWood() const override;
                bool isFire() const override;
                bool isTNT() const override;
                bool can_make_waves() override;
                bool can_overflow() override;
                bool dirt_can_push_through() const override;
                bool can_pass_through_it(Particle* blocking_particle) override;
                bool can_be_pushed_into_by_water() const override;
                bool water_can_push_it_to_the_left() override;
                bool water_can_push_it_to_the_right() override;
                bool water_can_push_it_upwards() override;
                int water_to_the_left() override;
                int water_to_the_right() override;
                int water_from_to_the_left() override;
                int water_from_to_the_right() override;
                void accept(WorldVisitor& visitor) const override;
                void step() override;
                void particle_is_pushing_onto(Particle* particle) override;
                void push_onto_dirt(DirtParticle* dirt_particle) override;
                void push_onto_void(VoidParticle* void_particle) override;
                void push_onto_water(WaterParticle* water_particle) override;
                void push_onto_mud(MudParticle* mud_particle) override;
                void push_onto_grass(GrassParticle* grass_particle) override;
                void push_onto_stone(StoneParticle* stone_particle) override;
                void push_onto_wood(WoodParticle* wood_particle) override;
                void push_onto_fire(FireParticle* fire_particle) override;
                void dirt_pushing_to_the_left(DirtParticle* dirt_particle) override;
                void dirt_pushing_to_the_right(DirtParticle* dirt_particle) override;
                void mud_pushing_to_the_left(MudParticle* mud_particle) override;
                void mud_pushing_to_the_right(MudParticle* mud_particle) override;
                void water_pushing_to_the_left(WaterParticle* water_particle) override;
                void water_pushing_to_the_right(WaterParticle* water_particle) override;
                void grass_trying_to_spread_onto() override;
                void burn() override;

};

class MudParticle : public Particle {
        public:

                MudParticle(World* world);
                bool isDirt() const override;
                bool isGrass() const override;
                bool isVoid() const override;
                bool isWater() const override;
                bool isMud() const override;
                bool isStone() const override;
                bool isWood() const override;
                bool isFire() const override;
                bool isTNT() const override;
                bool dirt_can_push_through() const override;
                bool can_pass_through_it(Particle* blocking_particle) override;
                bool can_be_pushed_into_by_water() const override;
                bool water_can_push_it_to_the_left() override;
                bool water_can_push_it_to_the_right() override;
                bool water_can_push_it_upwards() override;
                int water_to_the_left() override;
                int water_to_the_right() override;
                int water_from_to_the_left() override;
                int water_from_to_the_right() override;
                void accept(WorldVisitor& visitor) const override;
                void step() override;
                void particle_is_pushing_onto(Particle* particle) override;
                void push_onto_dirt(DirtParticle* dirt_particle) override;
                void push_onto_void(VoidParticle* void_particle) override;
                void push_onto_water(WaterParticle* water_particle) override;
                void push_onto_mud(MudParticle* mud_particle) override;
                void push_onto_fire(FireParticle* fire_particle) override;
                void dirt_pushing_to_the_left(DirtParticle* dirt_particle) override;
                void dirt_pushing_to_the_right(DirtParticle* dirt_particle) override;
                void mud_pushing_to_the_left(MudParticle* mud_particle) override;
                void mud_pushing_to_the_right(MudParticle* mud_particle) override;
                void water_pushing_to_the_left(WaterParticle* water_particle) override;
                void water_pushing_to_the_right(WaterParticle* water_particle) override;
                void grass_trying_to_spread_onto() override;
                void burn() override;

};

class StoneParticle : public Particle {
        public:

                StoneParticle(World* world);
                bool isDirt() const override;
                bool isGrass() const override;
                bool isVoid() const override;
                bool isWater() const override;
                bool isMud() const override;
                bool isStone() const override;
                bool isWood() const override;
                bool isFire() const override;
                bool isTNT() const override;
                bool isDam() override;
                bool isDamUpwards() override;
                bool isDamDownwards() override;
                bool dirt_can_push_through() const override;
                bool can_pass_through_it(Particle* blocking_particle) override;
                bool can_be_pushed_into_by_water() const override;
                bool water_can_push_it_to_the_left() override;
                bool water_can_push_it_to_the_right() override;
                bool water_can_push_it_upwards() override;
                int water_to_the_left() override;
                int water_to_the_right() override;
                int water_from_to_the_left() override;
                int water_from_to_the_right() override;
                void accept(WorldVisitor& visitor) const override;
                void step() override;
                void particle_is_pushing_onto(Particle* particle) override;
                void push_onto_dirt(DirtParticle* dirt_particle) override;
                void push_onto_void(VoidParticle* void_particle) override;
                void push_onto_water(WaterParticle* water_particle) override;
                void push_onto_mud(MudParticle* mud_particle) override;
                void push_onto_fire(FireParticle* fire_particle) override;
                void dirt_pushing_to_the_left(DirtParticle* dirt_particle) override;
                void dirt_pushing_to_the_right(DirtParticle* dirt_particle) override;
                void mud_pushing_to_the_left(MudParticle* mud_particle) override;
                void mud_pushing_to_the_right(MudParticle* mud_particle) override;
                void water_pushing_to_the_left(WaterParticle* water_particle) override;
                void water_pushing_to_the_right(WaterParticle* water_particle) override;
                void grass_trying_to_spread_onto() override;
                void burn() override;
                void support(StoneParticle* stone_particle) override;

};

class WoodParticle : public Particle {
        public:

                WoodParticle(World* world);
                bool isDirt() const override;
                bool isGrass() const override;
                bool isVoid() const override;
                bool isWater() const override;
                bool isMud() const override;
                bool isStone() const override;
                bool isWood() const override;
                bool isFire() const override;
                bool isTNT() const override;
                bool dirt_can_push_through() const override;
                bool can_pass_through_it(Particle* blocking_particle) override;
                bool can_float_on_water() override;
                bool can_be_pushed_into_by_water() const override;
                bool water_can_push_it_to_the_left() override;
                bool water_can_push_it_to_the_right() override;
                bool water_can_push_it_upwards() override;
                int water_to_the_left() override;
                int water_to_the_right() override;
                int water_from_to_the_left() override;
                int water_from_to_the_right() override;
                void accept(WorldVisitor& visitor) const override;
                void step() override;
                void particle_is_pushing_onto(Particle* particle) override;
                void push_onto_dirt(DirtParticle* dirt_particle) override;
                void push_onto_void(VoidParticle* void_particle) override;
                void push_onto_water(WaterParticle* water_particle) override;
                void push_onto_mud(MudParticle* mud_particle) override;
                void dirt_pushing_to_the_left(DirtParticle* dirt_particle) override;
                void dirt_pushing_to_the_right(DirtParticle* dirt_particle) override;
                void mud_pushing_to_the_left(MudParticle* mud_particle) override;
                void mud_pushing_to_the_right(MudParticle* mud_particle) override;
                void water_pushing_to_the_left(WaterParticle* water_particle) override;
                void water_pushing_to_the_right(WaterParticle* water_particle) override;
                void grass_trying_to_spread_onto() override;
                void burn() override;

        private:
                bool isDam() override;
                bool isDamUpwards() override;
                bool isDamDownwards() override;
                int steps_until_burning_out = 35;

};

class FireParticle : public Particle {
        public:

                FireParticle(World* world);
                bool isDirt() const override;
                bool isGrass() const override;
                bool isVoid() const override;
                bool isWater() const override;
                bool isMud() const override;
                bool isStone() const override;
                bool isWood() const override;
                bool isFire() const override;
                bool isTNT() const override;
                bool dirt_can_push_through() const override;
                bool can_pass_through_it(Particle* blocking_particle) override;
                bool dirt_can_pass_through_it() override;
                bool grass_can_pass_through_it() override;
                bool water_can_pass_through_it() override;
                bool mud_can_pass_through_it() override;
                bool stone_can_pass_through_it() override;
                bool wood_can_pass_through_it() override;
                bool fire_can_pass_through_it() override;
                bool can_be_pushed_into_by_water() const override;
                bool water_can_push_it_to_the_left() override;
                bool water_can_push_it_to_the_right() override;
                int water_to_the_left() override;
                int water_to_the_right() override;
                int water_from_to_the_left() override;
                int water_from_to_the_right() override;
                void accept(WorldVisitor& visitor) const override;
                void step() override;
                void particle_is_pushing_onto(Particle* particle) override;
                void push_onto_dirt(DirtParticle* dirt_particle) override;
                void push_onto_void(VoidParticle* void_particle) override;
                void push_onto_water(WaterParticle* water_particle) override;
                void push_onto_mud(MudParticle* mud_particle) override;
                void push_onto_grass(GrassParticle* grass_particle) override;
                void dirt_pushing_to_the_left(DirtParticle* dirt_particle) override;
                void dirt_pushing_to_the_right(DirtParticle* dirt_particle) override;
                void mud_pushing_to_the_left(MudParticle* mud_particle) override;
                void mud_pushing_to_the_right(MudParticle* mud_particle) override;
                void water_pushing_to_the_left(WaterParticle* water_particle) override;
                void water_pushing_to_the_right(WaterParticle* water_particle) override;
                void grass_trying_to_spread_onto() override;
                void burn() override;

        private:

                int steps_until_being_put_off = 50;
};

class TNTParticle : public Particle {
        public:

                TNTParticle(World* world);
                bool isDirt() const override;
                bool isGrass() const override;
                bool isVoid() const override;
                bool isWater() const override;
                bool isMud() const override;
                bool isStone() const override;
                bool isWood() const override;
                bool isFire() const override;
                bool isTNT() const override;
                bool dirt_can_push_through() const override;
                bool can_pass_through_it(Particle* blocking_particle) override;
                bool can_be_pushed_into_by_water() const override;
                bool water_can_push_it_to_the_left() override;
                bool water_can_push_it_to_the_right() override;
                int water_to_the_left() override;
                int water_to_the_right() override;
                int water_from_to_the_left() override;
                int water_from_to_the_right() override;
                void accept(WorldVisitor& visitor) const override;
                void step() override;
                void particle_is_pushing_onto(Particle* particle) override;
                void push_onto_dirt(DirtParticle* dirt_particle) override;
                void push_onto_void(VoidParticle* void_particle) override;
                void push_onto_water(WaterParticle* water_particle) override;
                void push_onto_mud(MudParticle* mud_particle) override;
                void push_onto_grass(GrassParticle* grass_particle) override;
                void push_onto_stone(StoneParticle* stone_particle) override;
                void push_onto_wood(WoodParticle* wood_particle) override;
                void push_onto_fire(FireParticle* fire_particle) override;
                void dirt_pushing_to_the_left(DirtParticle* dirt_particle) override;
                void dirt_pushing_to_the_right(DirtParticle* dirt_particle) override;
                void mud_pushing_to_the_left(MudParticle* mud_particle) override;
                void mud_pushing_to_the_right(MudParticle* mud_particle) override;
                void water_pushing_to_the_left(WaterParticle* water_particle) override;
                void water_pushing_to_the_right(WaterParticle* water_particle) override;
                void grass_trying_to_spread_onto() override;
                void burn() override;
};
