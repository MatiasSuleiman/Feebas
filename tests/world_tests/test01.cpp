#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test01 world is empty when initialized") {
        World world;
        REQUIRE(world.there_is_void_particle_at(0, 0));
        REQUIRE_FALSE(world.there_is_dirt_particle_at(0,0));

}
