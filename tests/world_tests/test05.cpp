#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test05 world doesnt let particles fall throug the left walls") {
        World world;
        world.Create_dirt_particle_at(0,0);
        world.Create_dirt_particle_at(0,1);
        world.step();
        REQUIRE(world.there_is_dirt_particle_at(1,0));
}

TEST_CASE("test05b world doesnt let particles fall throug the right walls") {
        World world;
        world.Create_dirt_particle_at(799,0);
        world.Create_dirt_particle_at(799,1);
        world.step();
        REQUIRE(world.there_is_dirt_particle_at(798,0));
}
