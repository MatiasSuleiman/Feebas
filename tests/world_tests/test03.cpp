#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test03 world lets advance simulation one step") {
        World world;
        world.Create_dirt_particle_at(400,300);
        REQUIRE(world.there_is_dirt_particle_at(400, 300));
        world.step();
        REQUIRE(world.there_is_dirt_particle_at(399,300));
}
