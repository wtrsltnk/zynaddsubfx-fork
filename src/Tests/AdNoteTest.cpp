#include "../Params/Controller.h"
#include "../globals.h"
#include "../Synth/ADnote.h"
#include "../Misc/Master.h"
#include "../DSP/FFTwrapper.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <string.h>

SYNTH_T* synth;

TEST_CASE( "Bounds Check", "[adnote]" )
{
    ADnote       *note;
    Master       *master;
    FFTwrapper   *fft;
    Controller   *controller;
    unsigned char testnote;
    float *outR, *outL;

    //        void setUp() {
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

    //phew, glad to get thouse out of my way. took me a lot of sweat and gdb to get this far...

    fft = new FFTwrapper(synth->oscilsize);
    //prepare the default settings
    ADnoteParameters *defaultPreset = new ADnoteParameters(fft);

    //Assert defaults
    REQUIRE(!defaultPreset->VoicePar[1].Enabled);

    XMLwrapper *wrap = new XMLwrapper();
    wrap->loadXMLfile(std::string(SOURCE_DIR) + std::string("/guitar-adnote.xmz"));
    REQUIRE(wrap->enterbranch("MASTER"));
    REQUIRE(wrap->enterbranch("PART", 0));
    REQUIRE(wrap->enterbranch("INSTRUMENT"));
    REQUIRE(wrap->enterbranch("INSTRUMENT_KIT"));
    REQUIRE(wrap->enterbranch("INSTRUMENT_KIT_ITEM", 0));
    REQUIRE(wrap->enterbranch("ADD_SYNTH_PARAMETERS"));
    defaultPreset->getfromXML(wrap);
    //defaultPreset->defaults();

    //verify xml was loaded
    REQUIRE(defaultPreset->VoicePar[1].Enabled);



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

    delete defaultPreset;
    delete wrap;
    delete controller;
    delete synth;

    delete note;
    delete controller;
    delete fft;
    delete [] outL;
    delete [] outR;
    delete [] denormalkillbuf;
    FFT_cleanup();
    delete synth;
}

//TEST_CASE( "Test Defaults", "[adnote]" )
//{
//    ADnote       *note;
//    Master       *master;
//    FFTwrapper   *fft;
//    Controller   *controller;
//    unsigned char testnote;
//    float *outR, *outL;

//    synth = new SYNTH_T();

//    int sampleCount = 0;

//    note->noteout(outL, outR);
//    sampleCount += synth->buffersize;

//    REQUIRE((outL[255] - 0.254609f) < 0.0001f);

//    note->relasekey();


//    note->noteout(outL, outR);
//    sampleCount += synth->buffersize;
//    REQUIRE((outL[255] - -0.102197f) < 0.0001f);

//    note->noteout(outL, outR);
//    sampleCount += synth->buffersize;
//    REQUIRE((outL[255] - -0.111261f) < 0.0001f);

//    note->noteout(outL, outR);
//    sampleCount += synth->buffersize;
//    REQUIRE((outL[255] - -0.021375f) < 0.0001f);

//    note->noteout(outL, outR);
//    sampleCount += synth->buffersize;
//    REQUIRE((outL[255] - 0.149149f) < 0.0001f);

//    while(!note->finished()) {
//        note->noteout(outL, outR);
//        sampleCount += synth->buffersize;
//    }

//    REQUIRE(sampleCount == 9472);

//    delete note;
//    delete controller;
//    delete fft;
//    delete [] outL;
//    delete [] outR;
//    delete [] denormalkillbuf;
//    FFT_cleanup();
//    delete synth;
//}

//#define OUTPUT_PROFILE
//#ifdef OUTPUT_PROFILE
//TEST_CASE( "Test speeds", "[adnote]" )
//{
//    ADnote       *note;
//    float *outR, *outL;

//    const int samps = 15000;

//    int t_on = clock(); // timer before calling func
//    for(int i = 0; i < samps; ++i)
//        note->noteout(outL, outR);
//    int t_off = clock(); // timer when func returns

//    printf("AdNoteTest: %f seconds for %d Samples to be generated.\n",
//           (static_cast<float>(t_off - t_on)) / CLOCKS_PER_SEC, samps);
//    delete [] outL;
//    delete [] outR;
//}
//#endif

