#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test34 TNT should should try to fall to the side of any solid") {
        World world;
        world.Create_dirt_particle_at(0,0);
        world.Create_TNT_particle_at(0,1);

        world.Create_grass_particle_at(3,0);
        world.Create_TNT_particle_at(3,1);

        world.Create_mud_particle_at(5,0);
        world.Create_TNT_particle_at(5,1);

        world.Create_stone_particle_at(7,0);
        world.Create_TNT_particle_at(7,1);

        world.Create_wood_particle_at(9,0);
        world.Create_TNT_particle_at(9,1);

        world.step();

        REQUIRE(world.there_is_TNT_particle_at(1,0));
        REQUIRE(world.there_is_TNT_particle_at(2,0));
        REQUIRE(world.there_is_TNT_particle_at(4,0));
        REQUIRE(world.there_is_TNT_particle_at(6,0));
        REQUIRE(world.there_is_TNT_particle_at(8,0));

}
