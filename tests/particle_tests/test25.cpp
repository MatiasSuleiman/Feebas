#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test24 fire should be put off after 25 steps of not consuming anything") {

        World world;

        world.Create_fire_particle_at(0,0);
        for(int i = 0; i < 25; i++){
                world.step();
        }

        REQUIRE_FALSE(world.there_is_fire_particle_at(0,0));

}
