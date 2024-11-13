#include "uboat/uboat.h"
#include <string>
#include <vector>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "common.h"

TEST_SUITE("Playlists") {
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

    TEST_CASE("Playlist lifecycle") {

        SUBCASE("createPlaylist") {
            auto songResult = client.getRandomSongs("3");
            REQUIRE(songResult.has_value());
            auto songs = songResult.value().song;

            std::vector<std::string> ids;

            for (const auto &song : songs)
                ids.emplace_back(song.id);

            REQUIRE_EQ(ids.size(), 3);

            auto result = client.createPlaylist("", "testPlaylist", ids);

            CHECK(result.has_value());
            CHECK_EQ(result.value().songCount, 3);
        }

        SUBCASE("get and deletePlaylist") {

            auto getResult = client.getPlaylists();

            CHECK(getResult.has_value());

            auto playlistId = getResult.value().playlist.at(0).id;

            auto result = client.deletePlaylist(playlistId);
            CHECK(result.has_value());
        }
    }
}
