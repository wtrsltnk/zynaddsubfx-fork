
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include "../Misc/Util.h"
#include "../Synth/ADnote.h"
#include "../Synth/OscilGen.h"
#include "../Params/Presets.h"
#include "../DSP/FFTwrapper.h"
#include "../globals.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

SYNTH_T *synth;

using namespace std;


#define BUF 256

TEST_CASE( "AdNote", "[subnote]" )
{
    ADnote       *note;
    FFTwrapper   *fft;
    Controller   *controller;
    unsigned char testnote;
    ADnoteParameters *params;
    float freq;

    float outR[BUF], outL[BUF];

    //First the sensible settings and variables that have to be set:
    synth = new SYNTH_T;
    synth->buffersize = BUF;

    memset(outL,0,sizeof(outL));
    memset(outR,0,sizeof(outR));

    //next the bad global variables that for some reason have not been properly placed in some
    //initialization routine, but rather exist as cryptic oneliners in main.cpp:
    denormalkillbuf = new float[BUF];
    memset(denormalkillbuf, 0, sizeof(float)*BUF);

    fft = new FFTwrapper(BUF);
    //prepare the default settings
    params = new ADnoteParameters(fft);

    //sawtooth to make things a bit more interesting
    params->VoicePar[0].OscilSmp->Pcurrentbasefunc = 3;

    controller = new Controller();

    //lets go with.... 50! as a nice note
    testnote = 50;
    freq = 440.0f * powf(2.0f, (testnote - 69.0f) / 12.0f);

    SECTION("testUnison") {
        sprng(0xbeef);

        float data[][4] = {
            {-0.034547f, 0.034349f, -0.000000f, 0.138284f},
            {0.023612f, -0.093842f, 0.000000f, -0.040384f},
            {-0.015980f, 0.001871f, -0.014463f, -0.000726f},
            {-0.040970f, -0.000275f, 0.000000f, -0.121016f},
            {0.019250f, -0.045252f, 0.000270f, 0.105372f},
            {-0.086575f, 0.001130f, -0.018921f, 0.001329f},
            {0.009203f, -0.006176f, 0.017344f, -0.003316f},
            {0.029411f, -0.000248f, -0.112797f, -0.012883f},
            {0.043657f, -0.014062f, -0.003374f, -0.071821f},
            {0.007973f, 0.068019f, -0.038900f, 0.047639f},
            {-0.002055f, 0.011170f, -0.058152f, -0.043493f},
            {-0.005298f, 0.000605f, -0.070932f, -0.005678f},
            {0.025028f, -0.027742f, 0.020985f, -0.015417f},
            {0.074349f, 0.000640f, 0.080613f, 0.066636f},
            {-0.045721f, 0.000279f, 0.009819f, 0.032202f},
        };

        int freq_spread[15];
        int stereo_spread[15];
        int vibrato[15];
        int vibrato_speed[15];
        int inv_phase[15];
        for(int i=0; i<15; ++i)
        {
            freq_spread[i]    = prng()%0x7f;
            stereo_spread[i]  = prng()%0x7f;
            vibrato[i]        = prng()%0x7f;
            vibrato_speed[i]  = prng()%0x7f;
            inv_phase[i]      = prng()%5;
        }

        for(int a=0; a<15; ++a)
        {
            sprng(0);
            params->set_unison_size_index(0,a);
            params->VoicePar[0].Unison_frequency_spread = freq_spread[a];
            params->VoicePar[0].Unison_stereo_spread    = stereo_spread[a];
            params->VoicePar[0].Unison_vibratto         = vibrato[a];
            params->VoicePar[0].Unison_vibratto_speed   = vibrato_speed[a];
            params->VoicePar[0].Unison_invert_phase     = inv_phase[a];

            note = new ADnote(params, controller, freq, 120, 0, testnote, false);
            note->noteout(outL, outR);
//            REQUIRE(abs(outL[80] - data[a][0]) < 1e-5);
            note->noteout(outL, outR);
//            REQUIRE(abs(outR[90] - data[a][1]) < 1e-5);
            note->noteout(outL, outR);
//            REQUIRE(abs(outL[20] - data[a][2]) < 1e-5);
            note->noteout(outL, outR);
//            REQUIRE(abs(outR[200] - data[a][3]) < 1e-5);
            delete note;
        }
    }

    delete controller;
    delete fft;
    delete [] denormalkillbuf;
    FFT_cleanup();
    delete synth;
    delete params;
}
