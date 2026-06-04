#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test26 fire should be put off immediatelly when in contact with water") {

        World world;

        world.Create_fire_particle_at(0,0);
        world.Create_water_particle_at(0,1);

        world.Create_fire_particle_at(2,1);
        world.Create_water_particle_at(2,0);

        world.Create_fire_particle_at(4,0);
        world.Create_water_particle_at(5,0);
        world.Create_dirt_particle_at(5,1);

        world.Create_stone_particle_at(8,0);
        world.Create_water_particle_at(9,0);
        world.Create_dirt_particle_at(9,1);
        world.Create_fire_particle_at(10,0);

        world.step();

        REQUIRE_FALSE(world.there_is_fire_particle_at(0,0));
        REQUIRE_FALSE(world.there_is_fire_particle_at(2,1));
        REQUIRE_FALSE(world.there_is_fire_particle_at(4,0));
        REQUIRE_FALSE(world.there_is_fire_particle_at(10,0));

}
