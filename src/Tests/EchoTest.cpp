#include "../Effects/Echo.h"
#include "../globals.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

SYNTH_T *synth;

using namespace std;

TEST_CASE( "Decay with Feedback", "[echo]" )
{
    Stereo<float *> *input;
    float *outR, *outL;
    Echo  *testFX;

    synth = new SYNTH_T;

    outL  = new float[synth->buffersize];
    for(int i = 0; i < synth->buffersize; ++i)
        outL[i] = 0.0f;
    outR = new float[synth->buffersize];
    for(int i = 0; i < synth->buffersize; ++i)
        outR[i] = 0.0f;
    input = new Stereo<float *>(new float[synth->buffersize],
                                new float[synth->buffersize]);
    for(int i = 0; i < synth->buffersize; ++i)
        input->l[i] = input->r[i] = 0.0f;
    testFX = new Echo(true, outL, outR, 44100, 256);

    //Make sure that the output will be zero at start
    //(given a zero input)
    testFX->out(*input);
    for(int i = 0; i < synth->buffersize; ++i) {
        REQUIRE(outL[i] < 0.0001f);
        REQUIRE(outR[i] < 0.0001f);
    }

    //flood with high input
    for(int i = 0; i < synth->buffersize; ++i)
        input->r[i] = input->l[i] = 1.0f;
    char FEEDBACK = 5;
    testFX->changepar(FEEDBACK, 127);
    for(int i = 0; i < 100; ++i)
        testFX->out(*input);
    for(int i = 0; i < synth->buffersize; ++i) {
        REQUIRE(outL[i] != 0.0f);
        REQUIRE(outR[i] != 0.0f);
    }
    float amp = abs(outL[0] + outR[0]) / 2;
    //reset input to zero
    for(int i = 0; i < synth->buffersize; ++i)
        input->r[i] = input->l[i] = 0.0f;

    //give the echo time to fade based upon zero input and high feedback
    for(int i = 0; i < 50; ++i)
        testFX->out(*input);
    REQUIRE(abs(outL[0] + outR[0]) / 2 <= amp);

    char DELAY = 2;
    testFX->changepar(DELAY, 127);

    //flood with high input
    for(int i = 0; i < synth->buffersize; ++i)
        input->r[i] = input->l[i] = 1.0f;

    for(int i = 0; i < 500; ++i)
        testFX->out(*input);
    for(int i = 0; i < synth->buffersize; ++i) {
        REQUIRE(outL[i] != 0.0f);
        REQUIRE(outR[i] != 0.0f);
    }
    //After making sure the internal buffer has a nonzero value
    //cleanup
    //Then get the next output, which should be zereoed out if DELAY
    //is large enough
    testFX->cleanup();
    testFX->out(*input);
    for(int i = 0; i < synth->buffersize; ++i) {
        REQUIRE(outL[i] < 0.0001f);
        REQUIRE(outR[i] < 0.0001f);
    }

    delete[] input->r;
    delete[] input->l;
    delete input;
    delete[] outL;
    delete[] outR;
    delete testFX;
    delete synth;
}
