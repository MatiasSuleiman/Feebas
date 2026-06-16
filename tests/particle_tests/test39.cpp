#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test39 friction should not apply in the middle of the air") {

        World world;
        world.Create_stone_particle_at(0,5);
        world.Create_stone_particle_at(1,5);
        world.Create_stone_particle_at(2,5);
        world.Create_TNT_particle_at(0,6);
        world.Create_fire_particle_at(1,6);
        world.Create_dirt_particle_at(2,6);


        world.step();

        REQUIRE(world.there_is_ignited_TNT_particle_at(0,6));

        for (size_t i = 0; i < 10; i++) {
                world.step();
        }

        REQUIRE_FALSE(world.there_is_TNT_particle_at(0,6));

        REQUIRE(world.there_is_dirt_particle_at(9,5));

        world.step();

        REQUIRE(world.there_is_dirt_particle_at(16,3));

}
