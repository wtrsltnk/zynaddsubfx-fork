
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include "../Misc/Master.h"
#include "../Misc/Util.h"
#include "../Synth/ADnote.h"
#include "../Params/Presets.h"
#include "../DSP/FFTwrapper.h"
#include "../globals.h"

#include "catch.hpp"

SYNTH_T* synth;
Master* master;

using namespace std;

TEST_CASE("ADnote")
{
    ADnote       *note;
    FFTwrapper   *fft;
    Controller   *controller;
    unsigned char testnote;


    float *outR, *outL;

    //First the sensible settings and variables that have to be set:
    synth = new SYNTH_T;
    synth->buffersize = 256;

    outL = new float[synth->buffersize];
    for(int i = 0; i < synth->buffersize; ++i)
        *(outL + i) = 0;
    outR = new float[synth->buffersize];
    for(int i = 0; i < synth->buffersize; ++i)
        *(outR + i) = 0;

    //next the bad global variables that for some reason have not been properly placed in some
    //initialization routine, but rather exist as cryptic oneliners in main.cpp:
    denormalkillbuf = new float[synth->buffersize];
    for(int i = 0; i < synth->buffersize; ++i)
        denormalkillbuf[i] = 0;

    master = new Master();

    //phew, glad to get thouse out of my way. took me a lot of sweat and gdb to get this far...

    fft = new FFTwrapper(synth->oscilsize);
    //prepare the default settings
    ADnoteParameters *defaultPreset = new ADnoteParameters(fft);

    SECTION("Assert defaults") {
        REQUIRE(defaultPreset->VoicePar[1].Enabled == false);
    }

    XMLwrapper *wrap = new XMLwrapper();
    wrap->loadXMLfile(string(SOURCE_DIR) + string("/guitar-adnote.xmz"));
    SECTION("testXmlLoad") {
        REQUIRE(wrap->enterbranch("MASTER"));
        REQUIRE(wrap->enterbranch("PART", 0));
        REQUIRE(wrap->enterbranch("INSTRUMENT"));
        REQUIRE(wrap->enterbranch("INSTRUMENT_KIT"));
        REQUIRE(wrap->enterbranch("INSTRUMENT_KIT_ITEM", 0));
        REQUIRE(wrap->enterbranch("ADD_SYNTH_PARAMETERS"));
    }
    defaultPreset->getfromXML(wrap);
    //defaultPreset->defaults();

    SECTION("verify xml was loaded") {
        REQUIRE(defaultPreset->VoicePar[1].Enabled == false);
    }

    controller = new Controller();

    //lets go with.... 50! as a nice note
    testnote = 50;
    float freq = 440.0f * powf(2.0f, (testnote - 69.0f) / 12.0f);

    note = new ADnote(defaultPreset,
                      controller,
                      freq,
                      120,
                      0,
                      testnote,
                      false);

    SECTION("testDefaults") {
        int sampleCount = 0;

        note->noteout(outL, outR);
        sampleCount += synth->buffersize;
//        REQUIRE(abs(outL[255] - 0.254609f) < 0.0001f);

        note->relasekey();

        note->noteout(outL, outR);
        sampleCount += synth->buffersize;
//        REQUIRE(abs(outL[255] - 0.102197f) < 0.0001f);

        note->noteout(outL, outR);
        sampleCount += synth->buffersize;
//        REQUIRE(abs(outL[255] - 0.111261f) < 0.0001f);

        note->noteout(outL, outR);
        sampleCount += synth->buffersize;
//        REQUIRE(abs(outL[255] - 0.021375f) < 0.0001f);

        note->noteout(outL, outR);
        sampleCount += synth->buffersize;
//        REQUIRE(abs(outL[255] - 0.149149f) < 0.0001f);

        while(!note->finished()) {
            note->noteout(outL, outR);
            sampleCount += synth->buffersize;
        }

        REQUIRE(sampleCount == 2304);
    }

//#define OUTPUT_PROFILE
#ifdef OUTPUT_PROFILE
        void testSpeed() {
            const int samps = 15000;

            int t_on = clock(); // timer before calling func
            for(int i = 0; i < samps; ++i)
                note->noteout(outL, outR);
            int t_off = clock(); // timer when func returns

            printf("AdNoteTest: %f seconds for %d Samples to be generated.\n",
                   (static_cast<float>(t_off - t_on)) / CLOCKS_PER_SEC, samps);
        }
#endif

    delete defaultPreset;
    delete wrap;
    delete note;
    delete controller;
    delete fft;
    delete [] outL;
    delete [] outR;
    delete [] denormalkillbuf;
    FFT_cleanup();
    delete synth;
}
