#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test09 water levels to the terrain") {
        World world;
        world.Create_water_partcile_at(1,1);
        world.Create_water_partcile_at(1,0);
        world.Create_water_partcile_at(2,0);
        world.Create_water_partcile_at(3,0);

        world.step();

        REQUIRE(world.there_is_water_particle_at(0,0));
        REQUIRE(world.there_is_water_particle_at(1,0));
        REQUIRE(world.there_is_water_particle_at(2,0));
        REQUIRE(world.there_is_water_particle_at(3,0));
}
