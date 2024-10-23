#include "uboat/uboat.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "common.h"

TEST_SUITE("Searching") {
    auto client = uboat::OSClient(TEST_SERVER, TEST_USERNAME, TEST_PASSWORD,
                                  TEST_CLIENT_NAME);

    TEST_CASE("test env check") {
        SUBCASE("server alive") {
            auto ping_result = client.ping();
            CHECK(ping_result.has_value());
            CHECK_EQ(ping_result.value().error.code, 40);
        }

        SUBCASE("auth successful") {
            auto auth_result = client.authenticate();
            REQUIRE(auth_result.has_value());
        }
    }

    TEST_CASE("search3") {
        SUBCASE("search all") {
            auto result = client.search3("");
            CHECK(result.has_value());
        }

        SUBCASE("search all with count limit") {
            auto result = client.search3("", "1");
            CHECK(result.has_value());
            CHECK_EQ(result.value().artist.size(),1);
        }

        SUBCASE("search with query") {
            auto result = client.search3("Harrison");
            CHECK(result.has_value());
        }
    }
}
