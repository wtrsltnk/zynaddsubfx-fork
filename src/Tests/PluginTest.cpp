
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include "../Misc/Master.h"
#include "../Misc/Util.h"
#include "../globals.h"

#include "catch.hpp"

SYNTH_T *synth;

using namespace std;

char *instance_name=(char*)"";

string loadfile(string fname)
{
    std::ifstream t(fname.c_str());
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    return str;
}

TEST_CASE("Plugin")
{
    float *outR, *outL;
    Master *master[16];

    synth = new SYNTH_T;
    synth->buffersize = 256;
    synth->samplerate = 48000;
    synth->alias();

    outL  = new float[1024];
    for(int i = 0; i < synth->buffersize; ++i)
        outL[i] = 0.0f;
    outR = new float[1024];
    for(int i = 0; i < synth->buffersize; ++i)
        outR[i] = 0.0f;

    //next the bad global variables that for some reason have not been properly placed in some
    //initialization routine, but rather exist as cryptic oneliners in main.cpp:
    denormalkillbuf = new float[synth->buffersize];
    for(int i = 0; i < synth->buffersize; ++i)
        denormalkillbuf[i] = 0;

    for(int i = 0; i < 16; ++i)
        master[i] = new Master();

    for(int x=0; x<100; ++x)
        for(int i=0; i<16; ++i)
            master[i]->GetAudioOutSamples(rand() % 1025, synth->samplerate, outL, outR);

    SECTION("testPanic") {
        master[0]->setController(0, 0x64, 0);
        master[0]->noteOn(0,64,64);
        master[0]->AudioOut(outL, outR);

        float sum = 0.0f;
        for(int i = 0; i < synth->buffersize; ++i)
            sum += fabs(outL[i]);

        REQUIRE(0.1f < sum);
    }

// This test is not working because my compiler (mingw) has a different way of printing small numbers
    SECTION("testLoadSave") {
        const string fname = string(SOURCE_DIR) + "/guitar-adnote.xmz";
        const string fdata = loadfile(fname);
        char *result = NULL;
        master[0]->putalldata((char*)fdata.c_str(), fdata.length());
        int res = master[0]->getalldata(&result);

        REQUIRE(fdata.length() == res);
        REQUIRE(fdata == result);
    }

    for(int i = 0; i < 16; ++i)
        delete master[i];

    delete[] outL;
    delete[] outR;
    delete synth;
};
