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

    TEST_CASE("setRating") {
        auto searchResult = client.search3("");

        REQUIRE(searchResult.has_value());

        auto album = searchResult.value().album.at(0);

        SUBCASE("rating") {
            auto result = client.setRating(album.id, "5");

            CHECK(result.has_value());

            auto rated = client.getAlbum(album.id);

            CHECK(rated.has_value());

            CHECK_EQ(rated.value().userRating, 5);
        }

        SUBCASE("remove rating") {
            auto result = client.setRating(album.id, "0");

            CHECK(result.has_value());

            auto removed = client.getAlbum(album.id);

            CHECK(removed.has_value());

            CHECK_EQ(removed.value().userRating, 0);
        }
    }

    TEST_CASE("scrobble") {
        auto searchResult = client.search3("");

        REQUIRE(searchResult.has_value());

        auto song = searchResult.value().song.at(0);

        SUBCASE("srcobble once") {
            auto result = client.scrobble(song.id);

            CHECK(result.has_value());

            auto songAfter = client.search3("").value().song.at(0);

            REQUIRE_EQ(song.id, songAfter.id);

            CHECK_EQ(songAfter.playCount, song.playCount + 1);
        }
    }
}
