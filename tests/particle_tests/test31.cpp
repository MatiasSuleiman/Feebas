#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test31 falling has a standard accelleration of 1 for all particles") {
        World world;
        world.Create_dirt_particle_at(0,100);

        world.step();
        REQUIRE(world.there_is_dirt_particle_at(0,99));

        world.step();
        REQUIRE(world.there_is_dirt_particle_at(0,97));

        world.step();
        REQUIRE(world.there_is_dirt_particle_at(0,94));

        world.step();
        REQUIRE(world.there_is_dirt_particle_at(0,90));

        world.step();
        REQUIRE(world.there_is_dirt_particle_at(0,85));

}
