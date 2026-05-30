#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test09 world lets create a mud particle") {

        World world;
        world.Create_mud_particle_at(0,0);
        REQUIRE(world.there_is_mud_particle_at(0, 0));
}
