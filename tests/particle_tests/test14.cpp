#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test14 falling through water is a standard behaviour for all non-liquid particles") {

        World world1;

        world1.Create_water_particle_at(1,0);
        world1.Create_water_particle_at(2,0);
        world1.Create_water_particle_at(3,0);
        world1.Create_dirt_particle_at(2,1);
        world1.step();

        REQUIRE(world1.there_is_water_particle_at(0,0));
        REQUIRE(world1.there_is_water_particle_at(1,0));
        REQUIRE(world1.there_is_mud_particle_at(2,0));
        REQUIRE(world1.there_is_water_particle_at(3,0));
        REQUIRE_FALSE(world1.there_is_water_particle_at(4,0));

        World world2;

        world2.Create_water_particle_at(1,0);
        world2.Create_water_particle_at(2,0);
        world2.Create_water_particle_at(3,0);
        world2.Create_mud_particle_at(2,1);
        world2.step();

        REQUIRE(world2.there_is_water_particle_at(0,0));
        REQUIRE(world2.there_is_water_particle_at(1,0));
        REQUIRE(world2.there_is_mud_particle_at(2,0));
        REQUIRE(world2.there_is_water_particle_at(3,0));
        REQUIRE_FALSE(world2.there_is_water_particle_at(4,0));


        World world3;

        world3.Create_water_particle_at(1,0);
        world3.Create_water_particle_at(2,0);
        world3.Create_water_particle_at(3,0);
        world3.Create_grass_particle_at(2,1);
        world3.step();

        REQUIRE(world3.there_is_water_particle_at(0,0));
        REQUIRE(world3.there_is_water_particle_at(1,0));
        REQUIRE(world3.there_is_grass_particle_at(2,0));
        REQUIRE(world3.there_is_water_particle_at(3,0));
        REQUIRE_FALSE(world3.there_is_water_particle_at(4,0));


}
