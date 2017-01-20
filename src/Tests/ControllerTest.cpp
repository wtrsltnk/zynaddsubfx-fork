

#include <iostream>
#include "../Params/Controller.h"
#include "../globals.h"

#include "catch.hpp"

SYNTH_T *synth;

TEST_CASE("Controller")
{
    Controller *testCtl;

    synth   = new SYNTH_T;
    testCtl = new Controller();

    SECTION("testPortamentoRange") {
        //Initialize portamento
        testCtl->setportamento(127);
        testCtl->portamento.time = 127;
        testCtl->initportamento(40.0f, 400.0f, false);
        //Bounds Check
        while(testCtl->portamento.used) {
            REQUIRE(0.0f <= testCtl->portamento.x);
            REQUIRE(testCtl->portamento.x <= 1.0f);
            REQUIRE(0.1f <= testCtl->portamento.freqrap);
            REQUIRE(testCtl->portamento.freqrap <= 1.0f);
            testCtl->updateportamento();
        }
        REQUIRE(0.0f <= testCtl->portamento.x);
        REQUIRE(testCtl->portamento.x <= 1.0f);
        REQUIRE(0.1f <= testCtl->portamento.freqrap);
        REQUIRE(testCtl->portamento.freqrap <= 1.0f);
    }

    SECTION("testPortamentoValue") {
        testCtl->setportamento(127);
        testCtl->portamento.time = 127;
        testCtl->initportamento(40.0f, 400.0f, false);
        int i;
        for(i = 0; i < 10; ++i)
            testCtl->updateportamento();
        //Assert that the numbers are the same as they were at release
        REQUIRE(abs(testCtl->portamento.x - 0.0290249f) < 0.000001f);
        REQUIRE(abs(testCtl->portamento.freqrap - 0.126122f) < 0.000001f);
    }

    delete testCtl;
    delete synth;
}
