#include "uboat/uboat.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "common.h"

TEST_SUITE("Browsing") {
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

    TEST_CASE("getGenres") {
        SUBCASE("get all") {
            auto result = client.getGenres();
            CHECK(result.has_value());
            CHECK_EQ(result.value().genre.size(), 2);
        }
    }

    TEST_CASE("getArtists") {
        SUBCASE("get all") {
            auto result = client.getArtists();
            CHECK(result.has_value());
            CHECK_EQ(result.value().index.size(), 2);
        }
    }

    TEST_CASE("getAlbum") {

        auto result = client.getAlbumList2("random");

        REQUIRE(result.has_value());

        auto albumlist = result.value().album;

        SUBCASE("with id") {
            auto id = albumlist.at(0).id;
            auto song_count = albumlist.at(0).songCount;
            auto result = client.getAlbum(id);
            CHECK(result.has_value());
            CHECK_EQ(id, result.value().id);
            CHECK_EQ(song_count, result.value().song.size());
        }

        SUBCASE("with wrong id") {
            auto id = "wrong";
            auto result = client.getAlbum(id);
            CHECK_FALSE(result.has_value());
            CHECK_EQ(result.error().code, 70);
        }
    }

    TEST_CASE("getArtistInfo2") {
        auto result = client.getArtists();

        REQUIRE(result.has_value());

        auto artists = result.value().index.at(0).artist;

        SUBCASE("with id") {
            auto id = artists.at(0).id;
            auto result = client.getArtistInfo2(id);
            CHECK(result.has_value());
        }

        SUBCASE("with wrong id") {
            auto id = "wrong";
            auto result = client.getArtistInfo2(id);
            CHECK_FALSE(result.has_value());
            CHECK_EQ(result.error().code, 70);
        }
    }

    TEST_CASE("getAlbumInfo2") {
        auto result = client.getAlbumList2("random");

        REQUIRE(result.has_value());

        auto albums = result.value().album;

        SUBCASE("with id") {
            auto id = albums.at(0).id;
            auto result = client.getAlbumInfo2(id);
            CHECK(result.has_value());
        }

        SUBCASE("with wrong id") {
            auto id = "wrong";
            auto result = client.getAlbumInfo2(id);
            CHECK_FALSE(result.has_value());
            CHECK_EQ(result.error().code, 70);
        }
    }
}
