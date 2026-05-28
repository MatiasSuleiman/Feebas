#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test04 grass particle falls if there is nothing underneath") {
        World world;
        world.Create_grass_particle_at(0,1);

        world.step();
        REQUIRE(world.there_is_grass_particle_at(0,0));
        
        world.step();
        REQUIRE(world.there_is_grass_particle_at(0,0));
}
