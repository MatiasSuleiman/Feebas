#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test35 TNT should ignite when in contact with fire") {
        World world;
        world.Create_fire_particle_at(0,0);
        world.Create_TNT_particle_at(1,0);

        world.Create_fire_particle_at(3,0);
        world.Create_TNT_particle_at(2,0);

        world.step();

        REQUIRE(world.there_is_ignited_TNT_particle_at(1,0));
        REQUIRE(world.there_is_ignited_TNT_particle_at(2,0));

}
