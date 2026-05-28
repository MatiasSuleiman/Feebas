#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test06 world lets create a grass particle") {
        World world;
        world.Create_grass_particle_at(0,0);
        REQUIRE(world.there_is_grass_particle_at(0, 0));
}
