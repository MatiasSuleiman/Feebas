#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test24 wood cannot be pushed to the side by water if it is stable") {

        World world;

        world.Create_wood_particle_at(1,1);
        world.Create_wood_particle_at(1,2);
        world.Create_wood_particle_at(1,3);

        world.Create_stone_particle_at(1,0);
        world.Create_stone_particle_at(1,4);
        world.Create_stone_particle_at(2,0);
        world.Create_stone_particle_at(3,0);
        world.Create_stone_particle_at(4,0);
        world.Create_stone_particle_at(5,0);
        world.Create_stone_particle_at(5,1);
        
        world.Create_water_particle_at(2,1);
        world.Create_water_particle_at(3,1);
        world.Create_water_particle_at(4,1);

        world.Create_dirt_particle_at(4,2);

        world.step();

        REQUIRE(world.there_is_wood_particle_at(1,1));
        REQUIRE(world.there_is_wood_particle_at(1,2));
        REQUIRE(world.there_is_wood_particle_at(1,3));
        REQUIRE_FALSE(world.there_is_wood_particle_at(0,1));

}
