#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test19 stone falls if it is unsupported by near stone") {

        World world;

        world.Create_stone_particle_at(1,0);

        world.step();

        REQUIRE_FALSE(world.there_is_stone_particle_at(0,0));
}
