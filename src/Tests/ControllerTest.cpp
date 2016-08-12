#include "../Params/Controller.h"
#include "../globals.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

SYNTH_T* synth;

TEST_CASE( "Bounds Check", "[controller]" )
{
    synth = new SYNTH_T();
    Controller* controller = new Controller();

    //Initialize portamento
    controller->setportamento(127);
    controller->portamento.time = 127;
    controller->initportamento(40.0f, 400.0f, false);

    //Bounds Check
    while (controller->portamento.used)
    {
        REQUIRE(0.0f <= controller->portamento.x);
        REQUIRE(controller->portamento.x <= 1.0f);
        REQUIRE(0.1f <= controller->portamento.freqrap);
        REQUIRE(controller->portamento.freqrap <= 1.0f);

        controller->updateportamento();
    }

    REQUIRE(0.0f <= controller->portamento.x);
    REQUIRE(controller->portamento.x <= 1.0f);
    REQUIRE(0.1f <= controller->portamento.freqrap);
    REQUIRE(controller->portamento.freqrap <= 1.0f);

    delete controller;
    delete synth;
}

TEST_CASE( "Assert that the numbers are the same as they were at release", "[controller]" )
{
    synth = new SYNTH_T();
    Controller* controller = new Controller();

    controller->setportamento(127);
    controller->portamento.time = 127;
    controller->initportamento(40.0f, 400.0f, false);

    for(int i = 0; i < 10; ++i)
        controller->updateportamento();

    REQUIRE((controller->portamento.x - 0.0290249f) < 0.000001f);
    REQUIRE((controller->portamento.freqrap - 0.126122f) < 0.000001f);

    delete controller;
    delete synth;
}
