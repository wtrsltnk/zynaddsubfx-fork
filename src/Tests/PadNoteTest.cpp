
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include "../Misc/Master.h"
#include "../Misc/Util.h"
#include "../Synth/PADnote.h"
#include "../Params/Presets.h"
#include "../DSP/FFTwrapper.h"
#include "../globals.h"

#include "catch.hpp"

SYNTH_T* synth;
Master* master;

using namespace std;

TEST_CASE("PadNote")
{
    PADnote      *note;
    FFTwrapper   *fft;
    Controller   *controller;
    unsigned char testnote;


    float *outR, *outL;

    synth = new SYNTH_T;
    //First the sensible settings and variables that have to be set:
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
    PADnoteParameters *defaultPreset = new PADnoteParameters(fft,NULL);
    XMLwrapper *wrap = new XMLwrapper();
    wrap->loadXMLfile(string(SOURCE_DIR) + string("/guitar-adnote.xmz"));
    SECTION("testXmlLoad"){
        REQUIRE(wrap->enterbranch("MASTER"));
        REQUIRE(wrap->enterbranch("PART", 2));
        REQUIRE(wrap->enterbranch("INSTRUMENT"));
        REQUIRE(wrap->enterbranch("INSTRUMENT_KIT"));
        REQUIRE(wrap->enterbranch("INSTRUMENT_KIT_ITEM", 0));
        REQUIRE(wrap->enterbranch("PAD_SYNTH_PARAMETERS"));
    }
    defaultPreset->getfromXML(wrap);

    defaultPreset->applyparameters(false);

    controller = new Controller();

    //lets go with.... 50! as a nice note
    testnote = 50;
    float freq = 440.0f * powf(2.0f, (testnote - 69.0f) / 12.0f);

    note = new PADnote(defaultPreset,
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

        REQUIRE((outL[255] - 0.0660f) < 0.0001f);

        note->relasekey();

        note->noteout(outL, outR);
        sampleCount += synth->buffersize;
//        REQUIRE(abs(outL[255] - -0.0729f) < 0.0001f);

        note->noteout(outL, outR);
        sampleCount += synth->buffersize;
//        REQUIRE(abs(outL[255] - 0.060818f) < 0.0001f);

        note->noteout(outL, outR);
        sampleCount += synth->buffersize;
//        REQUIRE(abs(outL[255] - 0.036895f) < 0.0005f);

        note->noteout(outL, outR);
        sampleCount += synth->buffersize;
//        REQUIRE(abs(outL[255] - -0.006623f) < 0.0001f);

        while(!note->finished()) {
            note->noteout(outL, outR);
            sampleCount += synth->buffersize;
        }

        REQUIRE(sampleCount == 2304);
    }

    SECTION("testSpeed") {
        const int samps = 15000;

        int t_on = clock(); // timer before calling func
        for(int i = 0; i < samps; ++i)
            note->noteout(outL, outR);
        int t_off = clock(); // timer when func returns

        printf("PadNoteTest: %f seconds for %d Samples to be generated.\n",
               (static_cast<float>(t_off - t_on)) / CLOCKS_PER_SEC, samps);
    }

    delete defaultPreset;
    delete note;
    delete controller;
    delete fft;
    delete [] outL;
    delete [] outR;
    delete [] denormalkillbuf;
    FFT_cleanup();
    delete synth;
}
