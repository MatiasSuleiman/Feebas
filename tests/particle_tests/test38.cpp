#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test38 TNT explosions push away farther particles with less force") {

        World world;
        world.Create_stone_particle_at(6,0);
        world.Create_stone_particle_at(11,0);
        world.Create_TNT_particle_at(9,0);
        world.Create_fire_particle_at(9,1);

        world.step();

        REQUIRE(world.there_is_ignited_TNT_particle_at(9,0));

        for (size_t i = 0; i < 10; i++) {
                world.step();
        }

        REQUIRE_FALSE(world.there_is_TNT_particle_at(9,0));

        REQUIRE(world.there_is_stone_particle_at(17,0));
        world.step();

        REQUIRE(world.there_is_stone_particle_at(1,0));

}
