#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test05 grass particle spreads to the side dirt particles after 50 ticks") {
        World world;
        world.Create_dirt_particle_at(0,0);
        world.Create_grass_particle_at(1,0);
        world.Create_dirt_particle_at(2,0);

        for(int i = 0; i < 50; i++){
                world.step();
        }

        REQUIRE(world.there_is_grass_particle_at(0,0));
        REQUIRE(world.there_is_grass_particle_at(1,0));
        REQUIRE(world.there_is_grass_particle_at(2,0));
        
}
