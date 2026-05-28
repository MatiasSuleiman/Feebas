#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test08 world lets create a water particle") {
        World world;
        world.Create_water_particle_at(0,0);
        REQUIRE(world.there_is_water_particle_at(0, 0));
}
