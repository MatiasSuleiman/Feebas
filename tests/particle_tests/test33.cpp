
#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test33 TNT should behave like a non-floatable solid") {
        World world;
        world.Create_water_particle_at(0,0);
        world.Create_water_particle_at(1,0);
        world.Create_dirt_particle_at(2,0);
        world.Create_TNT_particle_at(1,2);

        world.Create_water_particle_at(5,0);
        world.Create_water_particle_at(6,0);
        world.Create_TNT_particle_at(5,1);

        world.step();
        world.step();
        world.step();

        REQUIRE(world.there_is_water_particle_at(0,2));
        REQUIRE(world.there_is_water_particle_at(4,0));
        REQUIRE(world.there_is_TNT_particle_at(1,0));
        REQUIRE(world.there_is_TNT_particle_at(5,0));
        REQUIRE_FALSE(world.there_is_TNT_particle_at(1,2));
        REQUIRE_FALSE(world.there_is_TNT_particle_at(5,1));

}
