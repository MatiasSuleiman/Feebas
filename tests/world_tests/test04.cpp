#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test04 world doesnt let particles fall over the floor") {
        World world;
        world.Create_dirt_particle_at(0,0);
        REQUIRE(world.there_is_dirt_particle_at(0, 0));
        world.step();
        REQUIRE(world.there_is_dirt_particle_at(0,0));
}
