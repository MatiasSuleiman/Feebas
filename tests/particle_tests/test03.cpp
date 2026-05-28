#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test03 dirt particle falls to the right if it cant fall below or left") {
        World world;
        world.Create_dirt_particle_at(0,0);
        world.Create_dirt_particle_at(1,0);
        world.Create_dirt_particle_at(1,1);

        REQUIRE(world.there_is_dirt_particle_at(1,1));
        world.step();
        REQUIRE(world.there_is_dirt_particle_at(2,0));
}
