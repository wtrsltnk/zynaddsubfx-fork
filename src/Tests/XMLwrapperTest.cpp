
#include "../Misc/XMLwrapper.h"
#include <string>
#include "../globals.h"

#include "catch.hpp"

SYNTH_T *synth;

using namespace std;

TEST_CASE("XMLWrapper")
{
    XMLwrapper xmla;
    XMLwrapper xmlb;

    SECTION("testAddPar") {
        xmla.addpar("my Pa*_ramet@er", 75);
        REQUIRE(xmla.getpar("my Pa*_ramet@er", 0, -200, 200) == 75);
    }

    //here to verify that no leaks occur
    SECTION("testLoad") {
        string location = string(SOURCE_DIR) + string("/guitar-adnote.xmz");
        int res = xmla.loadXMLfile(location);
        REQUIRE(res == 0);

        SECTION("testContent") {
            REQUIRE(xmla.enterbranch("MASTER"));
            REQUIRE(xmla.enterbranch("PART", 1));
            REQUIRE(xmla.enterbranch("INSTRUMENT"));
            REQUIRE(xmla.enterbranch("INSTRUMENT_KIT"));
            REQUIRE(xmla.enterbranch("INSTRUMENT_KIT_ITEM", 0));
            REQUIRE(xmla.enterbranch("SUB_SYNTH_PARAMETERS"));
        }
    }

    SECTION("testAnotherLoad") {
        string data =
            "\n<?xml version=\"1.0f\" encoding=\"UTF-8\"?>\n\
<!DOCTYPE ZynAddSubFX-data>\n\
<ZynAddSubFX-data version-major=\"2\" version-minor=\"4\"\n\
version-revision=\"4\" ZynAddSubFX-author=\"Nasca Octavian Paul\">\n\
</ZynAddSubFX-data>\n";
        bool res = xmlb.putXMLdata(data.c_str());
        REQUIRE(res == true);
    }
};
