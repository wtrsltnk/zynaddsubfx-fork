
#include "../Misc/XMLwrapper.h"
#include <string>
#include "../globals.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

SYNTH_T *synth;

using namespace std;

TEST_CASE( "AdNote", "[subnote]" )
{
    XMLwrapper *xmla;
    XMLwrapper *xmlb;

    xmla = new XMLwrapper;
    xmlb = new XMLwrapper;

    SECTION("testAddPar") {
        xmla->addpar("my Pa*_ramet@er", 75);
        REQUIRE(xmla->getpar("my Pa*_ramet@er", 0, -200, 200) == 75);
    }

    //here to verify that no leaks occur
    SECTION("testLoad") {
        string location = string(SOURCE_DIR) + string("/Tests/guitar-adnote.xmz");
        xmla->loadXMLfile(location);
    }

    SECTION("testAnotherLoad") {
        string dat =
            "\n<?xml version=\"1.0f\" encoding=\"UTF-8\"?>\n\
<!DOCTYPE ZynAddSubFX-data>\n\
<ZynAddSubFX-data version-major=\"2\" version-minor=\"4\"\n\
version-revision=\"1\" ZynAddSubFX-author=\"Nasca Octavian Paul\">\n\
</ZynAddSubFX-data>\n";
        xmlb->putXMLdata(dat.c_str());
    }

    delete xmla;
    delete xmlb;
};
