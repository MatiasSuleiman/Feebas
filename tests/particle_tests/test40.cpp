#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include "world.hpp"

TEST_CASE("test40 TNT should make other TNTs in its core explosion radius ignite") {

        World world;
        world.Create_TNT_particle_at(0,0);
        world.Create_TNT_particle_at(2,0);
        world.Create_fire_particle_at(0,1);

        world.step();


        for (size_t i = 0; i < 10; i++) {
                world.step();
        }

        REQUIRE_FALSE(world.there_is_TNT_particle_at(0,0));
        REQUIRE(world.there_is_ignited_TNT_particle_at(8,0));

}
