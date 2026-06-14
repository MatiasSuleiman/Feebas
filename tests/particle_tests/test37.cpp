#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test37 TNT explosions destroy all surrounding neighbour particles") {
        World world;
        world.Create_stone_particle_at(0,0);
        world.Create_stone_particle_at(1,0);
        world.Create_stone_particle_at(2,0);
        world.Create_stone_particle_at(0,1);
        world.Create_stone_particle_at(0,2);
        world.Create_fire_particle_at(1,2);
        world.Create_stone_particle_at(2,2);
        world.Create_stone_particle_at(2,1);
        world.Create_TNT_particle_at(1,1);

        world.step();

        REQUIRE(world.there_is_ignited_TNT_particle_at(1,1));

        for (size_t i = 0; i < 10; i++) {
                world.step();
        }

        REQUIRE_FALSE(world.there_is_TNT_particle_at(1,1));

        REQUIRE_FALSE(world.there_is_stone_particle_at(0,0));
        REQUIRE_FALSE(world.there_is_stone_particle_at(1,0));
        REQUIRE_FALSE(world.there_is_stone_particle_at(2,0));
        REQUIRE_FALSE(world.there_is_stone_particle_at(0,1));
        REQUIRE_FALSE(world.there_is_stone_particle_at(0,2));
        REQUIRE_FALSE(world.there_is_stone_particle_at(1,2));
        REQUIRE_FALSE(world.there_is_stone_particle_at(2,2));
        REQUIRE_FALSE(world.there_is_stone_particle_at(2,1));
}
