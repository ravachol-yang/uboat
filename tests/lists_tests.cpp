#include "uboat/uboat.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "common.h"

TEST_SUITE("Album/Song Lists") {
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

    TEST_CASE("getAlbumList2") {
        SUBCASE("get random") {
            auto result = client.getAlbumList2("random",10);
            CHECK(result.has_value());
            CHECK_EQ(result.value().album.size(), 3);
        }
        SUBCASE("get byYear") {
            auto result = client.getAlbumList2("byYear",10,0,2000,2020);
            CHECK(result.has_value());
            CHECK_GT(result.value().album.at(0).year, 2000);
            CHECK_LT(result.value().album.at(0).year, 2020);
        }
        SUBCASE("get byGenre") {
            auto result = client.getAlbumList2("byGenre",10,0,0,0," ");
            CHECK(result.has_value());
        }
    }
}
