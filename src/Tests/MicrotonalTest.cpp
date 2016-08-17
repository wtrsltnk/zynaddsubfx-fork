
#include <iostream>
#include "../Misc/Microtonal.h"
#include <cstring>
#include <string>
#include <cstdio>
#include "../globals.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

SYNTH_T *synth;

using namespace std;

TEST_CASE( "", "[oscilgen]" )
{
    Microtonal *testMicro;
    synth     = new SYNTH_T;
    testMicro = new Microtonal();

    SECTION( "Verifies that the object is initialized correctly" ) {
        REQUIRE(testMicro->Pinvertupdown == 0);
        REQUIRE(testMicro->Pinvertupdowncenter == 60);
        REQUIRE(testMicro->getoctavesize() == 12);
        REQUIRE(testMicro->Penabled == 0);
        REQUIRE(testMicro->PAnote == 69);
        REQUIRE(testMicro->PAfreq == 440.0f);
        REQUIRE(testMicro->Pscaleshift == 64);
        REQUIRE(testMicro->Pfirstkey == 0);
        REQUIRE(testMicro->Plastkey == 127);
        REQUIRE(testMicro->Pmiddlenote == 60);
        REQUIRE(testMicro->Pmapsize == 12);
        REQUIRE(testMicro->Pmappingenabled == 0);
        REQUIRE(testMicro->Pglobalfinedetune == 64);

        REQUIRE(std::string((const char *)testMicro->Pname) == std::string("12tET"));
        REQUIRE(std::string((const char *)testMicro->Pcomment) == std::string("Equal Temperament 12 notes per octave"));

        for(int i = 0; i < 128; ++i)
            REQUIRE(testMicro->Pmapping[i] == i);

        REQUIRE((testMicro->getnotefreq(19, 0) - 24.4997f) < 0.0001f);
    }

    SECTION( "Tests saving/loading to XML" ) {
        //Gah, the XMLwrapper is a twisted maze
        testMicro->Penabled = 1;
        XMLwrapper xml;
        xml.beginbranch("Dummy"); //this should not be needed, but odd behavior
                                  //seems to exist from MICROTONAL being on the
                                  //top of the stack
        xml.beginbranch("MICROTONAL");
        testMicro->add2XML(&xml);
        xml.endbranch();
        xml.endbranch();

        char *tmp = xml.getXMLdata();
        Microtonal other;

        other.Penabled = 1;
        strcpy((char *)other.Pname, "Myname"); //will be nicer with strings

        REQUIRE(*testMicro != other); //sanity check

        REQUIRE(xml.enterbranch("Dummy"));
        REQUIRE(xml.enterbranch("MICROTONAL"));

        other.getfromXML(&xml);
        xml.exitbranch();
        xml.exitbranch();
        char *tmpo = xml.getXMLdata();

        REQUIRE(!strcmp(tmp, tmpo));
        free(tmp);
        free(tmpo);
    }

    delete testMicro;
    delete synth;
}
