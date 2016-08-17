
#include <cstdlib>
#include <cstdio>
#include "../Misc/Util.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

SYNTH_T *synth;

TEST_CASE( "RandGen", "[rand]" )
{
    SECTION("testPRNG") {
        REQUIRE((RND - 0.607781) < 0.00001);
        REQUIRE((RND - 0.591761) < 0.00001);
        REQUIRE((RND - 0.186133) < 0.00001);
        REQUIRE((RND - 0.286319) < 0.00001);
        REQUIRE((RND - 0.511766) < 0.00001);
    }
}
