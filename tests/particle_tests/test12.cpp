#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test12 dirt falling on water turns into mud") {

        World world;

        world.Create_dirt_particle_at(0,1);
        world.Create_water_particle_at(0,0);
        world.step();

        REQUIRE(world.there_is_mud_particle_at(0,0));

}
