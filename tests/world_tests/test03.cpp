#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test03 world lets advance simulation one step") {
        World world;
        world.Create_dirt_particle_at(0,1);
        REQUIRE(world.there_is_dirt_particle_at(0,1));
        world.step();
        REQUIRE(world.there_is_dirt_particle_at(0,0));
        REQUIRE(world.there_is_void_particle_at(0,1));
}
