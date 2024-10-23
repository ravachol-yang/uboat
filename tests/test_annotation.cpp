#include "uboat/uboat.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "common.h"

TEST_SUITE("Media annotation") {
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

    TEST_CASE("star and unstar") {

        auto randomSong = client.getRandomSongs();

        REQUIRE(randomSong.has_value());

        auto song = randomSong.value().song.at(0);

        SUBCASE("star song") {
            auto result = client.star(song.id);

            CHECK(result.has_value());
        }

        SUBCASE("unstar song") {
            auto result = client.unstar(song.id);

            CHECK(result.has_value());
        }

        SUBCASE("star album") {
            auto result = client.star("", song.parent);

            CHECK(result.has_value());

            auto album = client.getAlbum(song.parent);

            CHECK(album.has_value());

            CHECK_NE(album.value().starred, "");
        }

        SUBCASE("unstar album") {
            auto result = client.unstar("", song.parent);

            CHECK(result.has_value());

            auto album = client.getAlbum(song.parent);

            CHECK(album.has_value());

            CHECK_EQ(album.value().starred, "");
        }
    }
}
