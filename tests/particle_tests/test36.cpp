#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test36 TNT should explode 10 steps after igniting") {
        World world;
        world.Create_fire_particle_at(0,0);
        world.Create_TNT_particle_at(1,0);

        world.step();

        REQUIRE(world.there_is_ignited_TNT_particle_at(1,0));

        for (size_t i = 0; i < 10; i++) {
                world.step();
        }

        REQUIRE_FALSE(world.there_is_TNT_particle_at(1,0));
}
