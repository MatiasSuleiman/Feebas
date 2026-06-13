#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include <string>

#include "world.hpp"

TEST_CASE("test13 world API lets user create a TNT particle") {

        World world;
        world.Create_TNT_particle_at(0,0);

        REQUIRE(world.there_is_TNT_particle_at(0,0));
}
