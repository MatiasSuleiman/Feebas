#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test20 stone keeps its place if there is a stone nearby") {

        World world;

        world.Create_stone_particle_at(1,1);
        world.Create_stone_particle_at(1,2);


        world.Create_stone_particle_at(3,1);
        world.Create_stone_particle_at(4,1);


        world.Create_stone_particle_at(5,2);
        world.Create_stone_particle_at(6,1);


        world.Create_stone_particle_at(9,2);
        world.Create_stone_particle_at(8,1);

        world.step();

        REQUIRE(world.there_is_stone_particle_at(1,1));
        REQUIRE(world.there_is_stone_particle_at(1,2));

        REQUIRE(world.there_is_stone_particle_at(3,1));
        REQUIRE(world.there_is_stone_particle_at(4,1));

        REQUIRE(world.there_is_stone_particle_at(5,2));
        REQUIRE(world.there_is_stone_particle_at(6,1));

        REQUIRE(world.there_is_stone_particle_at(9,2));
        REQUIRE(world.there_is_stone_particle_at(8,1));
}
