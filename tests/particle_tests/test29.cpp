#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test29 fire spread instantly when touching grass") {

        World world;

        world.Create_grass_particle_at(0,0);
        world.Create_fire_particle_at(0,1);

        world.Create_grass_particle_at(2,1);
        world.Create_fire_particle_at(2,0);

        world.Create_grass_particle_at(4,0);
        world.Create_fire_particle_at(5,0);

        world.Create_grass_particle_at(8,0);
        world.Create_fire_particle_at(7,0);

        world.Create_grass_particle_at(10,0);
        world.Create_fire_particle_at(11,1);
        world.Create_stone_particle_at(11,0);

        world.Create_grass_particle_at(13,1);
        world.Create_stone_particle_at(13,0);
        world.Create_fire_particle_at(12,0);

        world.Create_grass_particle_at(15,1);
        world.Create_stone_particle_at(15,0);
        world.Create_fire_particle_at(16,0);

        world.Create_grass_particle_at(18,0);
        world.Create_fire_particle_at(17,1);
        world.Create_stone_particle_at(17,0);

        world.step();

        REQUIRE(world.there_is_fire_particle_at(0,0));
        REQUIRE(world.there_is_fire_particle_at(0,1));

        REQUIRE(world.there_is_fire_particle_at(2,1));
        REQUIRE(world.there_is_fire_particle_at(2,0));

        REQUIRE(world.there_is_fire_particle_at(4,0));
        REQUIRE(world.there_is_fire_particle_at(5,0));

        REQUIRE(world.there_is_fire_particle_at(8,0));
        REQUIRE(world.there_is_fire_particle_at(7,0));

        REQUIRE(world.there_is_fire_particle_at(10,0));
        REQUIRE(world.there_is_fire_particle_at(11,1));

        REQUIRE(world.there_is_fire_particle_at(13,1));
        REQUIRE(world.there_is_fire_particle_at(12,0));

        REQUIRE(world.there_is_fire_particle_at(15,1));
        REQUIRE(world.there_is_fire_particle_at(16,0));

        REQUIRE(world.there_is_fire_particle_at(18,0));
        REQUIRE(world.there_is_fire_particle_at(17,1));
}


