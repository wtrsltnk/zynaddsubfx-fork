
#include <string>
#include <ctime>
#include "../Synth/OscilGen.h"
#include "../globals.h"
#include "catch.hpp"

SYNTH_T *synth;

using namespace std;

TEST_CASE("OscilGen")
{
    float  freq;
    float *outR, *outL;
    FFTwrapper *fft;
    OscilGen   *oscil;

    synth = new SYNTH_T;
    //First the sensible settings and variables that have to be set:
    synth->buffersize = 256;
    synth->oscilsize  = 1024;

    outL = new float[synth->oscilsize];
    outR = new float[synth->oscilsize];
    memset(outL, 0, sizeof(float) * synth->oscilsize);
    memset(outR, 0, sizeof(float) * synth->oscilsize);

    //next the bad global variables that for some reason have not been properly placed in some
    //initialization routine, but rather exist as cryptic oneliners in main.cpp:
    denormalkillbuf = new float[synth->buffersize];
    for(int i = 0; i < synth->buffersize; ++i)
        denormalkillbuf[i] = 0;

    //prepare the default settings
    fft   = new FFTwrapper(synth->oscilsize);
    oscil = new OscilGen(fft, NULL);

    SECTION( "Verify loaded instrument" ) {
        XMLwrapper *wrap = new XMLwrapper();
        wrap->loadXMLfile(string(SOURCE_DIR)
                          + string("/guitar-adnote.xmz"));
        REQUIRE(wrap->enterbranch("MASTER"));
        REQUIRE(wrap->enterbranch("PART", 0));
        REQUIRE(wrap->enterbranch("INSTRUMENT"));
        REQUIRE(wrap->enterbranch("INSTRUMENT_KIT"));
        REQUIRE(wrap->enterbranch("INSTRUMENT_KIT_ITEM", 0));
        REQUIRE(wrap->enterbranch("ADD_SYNTH_PARAMETERS"));
        REQUIRE(wrap->enterbranch("VOICE", 0));
        REQUIRE(wrap->enterbranch("OSCIL"));
        oscil->getfromXML(wrap);
        delete wrap;
    }

    //lets go with.... 50! as a nice note
    const char testnote = 50;
    freq = 440.0f * powf(2.0f, (testnote - 69.0f) / 12.0f);

    SECTION( "verifies that initialization occurs" ) {
        oscil->get(outL, freq);
    }

//    SECTION( "Test output" ) {
//        oscil->get(outL, freq);
//        REQUIRE(abs(outL[23] - -0.044547f) < 0.0001f);
//        REQUIRE(abs(outL[129] - -0.018169f) < 0.0001f);
//        REQUIRE(abs(outL[586] - 0.045647f) < 0.0001f);
//        REQUIRE(abs(outL[1023] - -0.038334f) < 0.0001f);
//    }

//    SECTION( "Test spectrum" ) {
//        oscil->getspectrum(synth->oscilsize / 2, outR, 1);
//        REQUIRE(abs(outR[0] - 350.698059f) < 0.0001f);
//        REQUIRE(abs(outR[1] - 228.889267f) < 0.0001f);
//        REQUIRE(abs(outR[2] - 62.187931f) < 0.0001f);
//        REQUIRE(abs(outR[3] - 22.295225f) < 0.0001f);
//        REQUIRE(abs(outR[4] - 6.942001f) < 0.0001f);
//        REQUIRE(abs(outR[26] - 0.015110f) < 0.0001f);
//        REQUIRE(abs(outR[47] - 0.003425f) < 0.0001f);
//        REQUIRE(abs(outR[65] - 0.001293f) < 0.0001f);
//    }

    SECTION( "performance testing" ) {
        const int samps = 15000;

        int t_on = clock(); // timer before calling func
        for(int i = 0; i < samps; ++i)
            oscil->prepare();
        int t_off = clock(); // timer when func returns

        printf("OscilGenTest: %f seconds for %d prepares.\n",
               (static_cast<float>(t_off - t_on)) / CLOCKS_PER_SEC, samps);

        t_on = clock(); // timer before calling func
        for(int i = 0; i < samps; ++i)
            oscil->get(outL, freq);
        t_off = clock(); // timer when func returns

        printf("OscilGenTest: %f seconds for %d gets.\n",
               (static_cast<float>(t_off - t_on)) / CLOCKS_PER_SEC, samps);
    }

    delete oscil;
    delete fft;
    delete[] outL;
    delete[] outR;
    delete[] denormalkillbuf;
    FFT_cleanup();
    delete synth;
}
