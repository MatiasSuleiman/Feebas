#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test27 fire should be put off after a solid non-flammable particle falls onto it") {

        World world;

        world.Create_fire_particle_at(0,0);
        world.Create_dirt_particle_at(0,1);

        world.Create_fire_particle_at(2,0);
        world.Create_mud_particle_at(2,1);

        world.Create_fire_particle_at(4,0);
        world.Create_stone_particle_at(4,1);

        world.step();

        REQUIRE_FALSE(world.there_is_fire_particle_at(0,0));
        REQUIRE_FALSE(world.there_is_fire_particle_at(2,0));
        REQUIRE_FALSE(world.there_is_fire_particle_at(4,0));

}


