#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test23 wood can be pushed to the side by water") {

        World world;

        world.Create_wood_particle_at(1,0);
        world.Create_water_particle_at(2,0);
        world.Create_water_particle_at(3,0);
        world.Create_water_particle_at(4,0);
        world.Create_wood_particle_at(5,0);

        world.Create_dirt_particle_at(3,1);
        world.step();

        REQUIRE(world.there_is_wood_particle_at(0,0));
        REQUIRE(world.there_is_water_particle_at(1,0));

        world.Create_dirt_particle_at(4,1);

        world.step();

        REQUIRE(world.there_is_wood_particle_at(6,0));
        REQUIRE(world.there_is_water_particle_at(5,0));

}
