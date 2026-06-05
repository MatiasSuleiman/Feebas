#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test28 fire should be put off faster when over a humid particle") {

        World world;

        world.Create_mud_particle_at(0,0);
        world.Create_fire_particle_at(0,1);

        for (int i = 0; i < 6; i++) {
                world.step();
        }

        REQUIRE_FALSE(world.there_is_fire_particle_at(0,1));

}


