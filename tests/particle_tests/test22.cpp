#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test22 wood can be pushed upwards by water") {

        World world;

        world.Create_wood_particle_at(0,1);
        world.Create_water_particle_at(0,0);
        world.Create_water_particle_at(1,0);
        world.Create_dirt_particle_at(1,1);
        world.Create_stone_particle_at(2,0);

        world.step();

        REQUIRE(world.there_is_wood_particle_at(0,2));
        REQUIRE(world.there_is_water_particle_at(0,0));
        REQUIRE(world.there_is_water_particle_at(0,1));
        REQUIRE(world.there_is_mud_particle_at(1,0));

}
