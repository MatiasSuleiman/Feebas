#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include <string>

#include "world.hpp"

TEST_CASE("test12 world lets create a fire particle") {

        World world;
        world.Create_fire_particle_at(0,0);
        REQUIRE(world.there_is_fire_particle_at(0,0));
}
