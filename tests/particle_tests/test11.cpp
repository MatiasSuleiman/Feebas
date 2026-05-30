#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test11 mud falls just like dirt") {

        World world;

        world.Create_mud_particle_at(1,1);
        world.step();
        REQUIRE(world.there_is_mud_particle_at(1,0));

        world.Create_mud_particle_at(1,1);
        world.step();
        REQUIRE(world.there_is_mud_particle_at(0,0));
        REQUIRE(world.there_is_mud_particle_at(1,0));

        world.Create_mud_particle_at(1,1);
        world.step();
        REQUIRE(world.there_is_mud_particle_at(0,0));
        REQUIRE(world.there_is_mud_particle_at(1,0));
        REQUIRE(world.there_is_mud_particle_at(2,0));

}
