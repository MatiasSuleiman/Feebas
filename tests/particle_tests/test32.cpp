#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test32 water should overflow at the speed the solid fell onto it") {
        World world;

        world.Create_water_particle_at(0,0);
        world.Create_water_particle_at(1,0);
        world.Create_dirt_particle_at(2,0);
        world.Create_dirt_particle_at(1,2);

        world.step();
        world.step();
        world.step();

        REQUIRE(world.there_is_water_particle_at(0,2));

}
