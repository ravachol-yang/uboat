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

        SUBCASE("create get, and updatePlaylist") {
            auto songResult = client.getRandomSongs("5");
            REQUIRE(songResult.has_value());
            auto songs = songResult.value().song;

            std::vector<std::string> ids;

            for (const auto &song : songs)
                ids.emplace_back(song.id);

            REQUIRE_EQ(ids.size(), 5);

            auto result = client.createPlaylist(
                "", "testPlaylist", {ids.at(0), ids.at(1), ids.at(2)});

            CHECK(result.has_value());
            CHECK_EQ(result.value().songCount, 3);

            auto getResult = client.getPlaylist(result.value().id);

            CHECK(getResult.has_value());
            CHECK_EQ(getResult.value().name, "testPlaylist");

            auto update =
                client.updatePlaylist(result.value().id, "updated", "", "",
                                      {ids.at(3), ids.at(4)}, {"0"});

            CHECK(update.has_value());

            auto updateResult = client.getPlaylist(result.value().id);

            CHECK(updateResult.has_value());
            CHECK_EQ(updateResult.value().name, "updated");
            CHECK_EQ(updateResult.value().entry.size(), 4);
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
