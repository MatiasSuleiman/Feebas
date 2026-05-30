#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test08 water particle falls to the sides") {
        World world;
        world.Create_water_particle_at(1,1);
        world.Create_water_particle_at(1,0);

        world.step();

        REQUIRE(world.there_is_water_particle_at(0,0));
        REQUIRE_FALSE(world.there_is_water_particle_at(0,1));

        world.Create_water_particle_at(1,1);
        
        world.step();

        REQUIRE(world.there_is_water_particle_at(0,0));
        REQUIRE(world.there_is_water_particle_at(1,0));
        REQUIRE(world.there_is_water_particle_at(2,0));
}
