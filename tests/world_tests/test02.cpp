#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test02 world lets create a dirt_partcile") {
        World world;
        world.Create_dirt_particle_at(0,0);
        REQUIRE(world.there_is_dirt_particle_at(0, 0));
}
