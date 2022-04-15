#include <elrond-test.hpp>
#include <elrond-catch.hpp>
#include <sstream>

#include "OStreamDebugOut.hpp"

using elrond::runtime::OStreamDebugOut;

TEST_CASE("Test elrond::runtime::OStreamDebugOut")
{
    std::ostringstream oss;
    OStreamDebugOut dout(oss);

    SECTION("Check empty elrond::runtime::OStreamDebugOut::putLn()"){
        dout.putLn();
        CHECK(oss.str() == "\n");
        oss.flush();
        oss.str("");
    }

    SECTION("Check with char[]"){
        char str[] = "string";

        dout.put(str);
        CHECK(oss.str() == "string");
        oss.flush();
        oss.str("");

        dout.putLn(str);
        CHECK(oss.str() == "string\n");
        oss.flush();
        oss.str("");
    }

    SECTION("Check with elrond::String"){
        dout.put(elrond::String("string"));
        CHECK(oss.str() == "string");
        oss.flush();
        oss.str("");

        dout.putLn(elrond::String("string"));
        CHECK(oss.str() == "string\n");
        oss.flush();
        oss.str("");
    }

    SECTION("Check with [unsigned] char"){

        dout.put('c');
        CHECK(oss.str() == "c");
        oss.flush();
        oss.str("");

        dout.putLn('c');
        CHECK(oss.str() == "c\n");
        oss.flush();
        oss.str("");

        dout.put((unsigned char) 156);
        CHECK(oss.str() == elrond::String(1, (unsigned char) 156));
        oss.flush();
        oss.str("");

        dout.putLn((unsigned char) 156);
        CHECK(oss.str() == elrond::String(1, (unsigned char) 156) + "\n");
        oss.flush();
        oss.str("");
    }

    SECTION("Check with [unsigned] int"){

        dout.put((int) 0xFFFFFFFF);
        CHECK(oss.str() == "-1");
        oss.flush();
        oss.str("");

        dout.putLn((int) 0xFFFFFFFF);
        CHECK(oss.str() == "-1\n");
        oss.flush();
        oss.str("");

        dout.put((unsigned int) 0xFFFFFFFFFF);
        CHECK(oss.str() == "4294967295");
        oss.flush();
        oss.str("");

        dout.putLn((unsigned int) 0xFFFFFFFFFF);
        CHECK(oss.str() == "4294967295\n");
        oss.flush();
        oss.str("");
    }

    SECTION("Check with [unsigned] long"){

        dout.put((int) (long) 0xFFFFFFFFFFFFFFFF);
        CHECK(oss.str() == "-1");
        oss.flush();
        oss.str("");

        dout.putLn((int) (long) 0xFFFFFFFFFFFFFFFF);
        CHECK(oss.str() == "-1\n");
        oss.flush();
        oss.str("");

        dout.put((unsigned long) 0xFFFFFFFFFFFFFFFF);
        CHECK(oss.str() == "18446744073709551615");
        oss.flush();
        oss.str("");

        dout.putLn((unsigned long) 0xFFFFFFFFFFFFFFFF);
        CHECK(oss.str() == "18446744073709551615\n");
        oss.flush();
        oss.str("");
    }

    SECTION("Check with [unsigned] long"){

        dout.put((int) (long) 0xFFFFFFFFFFFFFFFF);
        CHECK(oss.str() == "-1");
        oss.flush();
        oss.str("");

        dout.putLn((int) (long) 0xFFFFFFFFFFFFFFFF);
        CHECK(oss.str() == "-1\n");
        oss.flush();
        oss.str("");

        dout.put((unsigned long) 0xFFFFFFFFFFFFFFFF);
        CHECK(oss.str() == "18446744073709551615");
        oss.flush();
        oss.str("");

        dout.putLn((unsigned long) 0xFFFFFFFFFFFFFFFF);
        CHECK(oss.str() == "18446744073709551615\n");
        oss.flush();
        oss.str("");
    }

    SECTION("Check with double"){
        dout.put(3.14159265359);
        CHECK(oss.str() == "3.14159");
        oss.flush();
        oss.str("");

        dout.putLn(3.14159265359);
        CHECK(oss.str() == "3.14159\n");
        oss.flush();
        oss.str("");
    }
}
