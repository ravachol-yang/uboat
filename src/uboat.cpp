//
// SPDX-License-Identifier: GPL-3.0-only
//

#include "uboat/uboat.h"
#include "cpr/api.h"
#include "cpr/cprtypes.h"
#include "cpr/parameters.h"
#include "cpr/response.h"
#include <expected>
#include <map>
#include <nlohmann/json_fwd.hpp>
#include <openssl/evp.h>
#include <ostream>
#include <random>
#include <sstream>
#include <string>

using namespace uboat;
using json = nlohmann::json;

OSClient::OSClient(const std::string &server_url, const std::string &username,
                   const std::string &password, const std::string &client_name)
    : m_server_url(server_url + "/rest/"), m_username(username),
      m_password(password), m_client_name(client_name) {};

// Generate MD5 token and try to ping() the server
std::expected<server::SubsonicResponse<server::Error>, server::Error>
OSClient::authenticate() {

    // generate random salt
    const std::string CHARACTERS =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> distribution(0,
                                                       CHARACTERS.size() - 1);

    for (size_t i = 0; i < 10; ++i) {
        m_salt += CHARACTERS[distribution(gen)];
    }

    // generate MD5
    auto password_salt = m_password + m_salt;

    // see openssl evp docs
    // https://docs.openssl.org/3.0/man3/EVP_DigestInit/#examples
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len, i;

    md = EVP_get_digestbyname("MD5");

    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex2(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, password_salt.c_str(),
                     strlen(password_salt.c_str()));

    EVP_DigestFinal_ex(mdctx, md_value, &md_len);

    std::stringstream result;

    for (i = 0; i < md_len; i++)
        result << std::hex << std::setw(2) << std::setfill('0')
               << (int)md_value[i];

    m_token = result.str();

    EVP_MD_CTX_free(mdctx);

    // test the credentails
    auto response = ping();
    if (response.has_value()) {
        if (response.value().status == "ok") {
            return response;
        } else
            return std::unexpected(response.value().error);
    } else
        return std::unexpected(response.error());
}

// API Endpoints:

// System

// Used to (only) test connectivity with the server.
std::expected<server::SubsonicResponse<server::Error>, server::Error>
OSClient::ping() const {
    auto response = get_req<server::Error>("ping", {}, "error");
    // if the request gets a response
    // there may still be errors such as wrong password
    return response;
}

// Get details about the software license.
std::expected<server::License, server::Error> OSClient::getLicense() const {
    auto response = get_req<server::License>("getLicense", {}, "license");
    if (response) {
        return check(response.value());
    } else {
        return std::unexpected(response.error());
    }
}

// Browsing

// Returns all genres
std::expected<misc::Genres, server::Error> OSClient::getGenres() const {
    auto response = get_req<misc::Genres>("getGenres", {}, "genres");
    // extract data
    if (response)
        return check(response.value());
    else
        return std::unexpected(response.error());
}

// Similar to getIndexes, but organizes music according to ID3 tags.
std::expected<artist::Artists, server::Error> OSClient::getArtists() const {
    auto response = get_req<artist::Artists>("getArtists", {}, "artists");
    // extract data
    if (response)
        return check(response.value());
    else
        return std::unexpected(response.error());
}

// Returns details for an album, including a list of songs. This method
// organizes music according to ID3 tags.
std::expected<album::AlbumID3WithSongs, server::Error>
OSClient::getAlbum(const std::string &id) const {
    auto response =
        get_req<album::AlbumID3WithSongs>("getAlbum", {{"id", id}}, "album");

    // extract data
    if (response)
        return check(response.value());
    else
        return std::unexpected(response.error());
}

// Returns artist info.
// Similar to getArtistInfo, but organizes music according to ID3 tags.
std::expected<artist::ArtistInfo2, server::Error>
OSClient::getArtistInfo2(const std::string &id, const std::string &count,
                         const std::string &includeNotPresent) const {
    // make params
    std::map<std::string, std::string> params{
        {"id", id}, {"count", count}, {"includeNotPresent", includeNotPresent}};

    auto response =
        get_req<artist::ArtistInfo2>("getArtistInfo2", params, "artistInfo2");

    // extract data
    if (response)
        return check(response.value());
    else
        return std::unexpected(response.error());
}

// Returns album info.
// Similar to getAlbumInfo, but organizes music according to ID3 tags.
std::expected<album::AlbumInfo, server::Error>
OSClient::getAlbumInfo2(const std::string &id) const {
    auto response =
        get_req<album::AlbumInfo>("getAlbumInfo2", {{"id", id}}, "albumInfo");

    // extract data
    if (response)
        return check(response.value());
    else
        return std::unexpected(response.error());
}

std::expected<media::SimilarSongs2, server::Error>
OSClient::getSimilarSongs2(const std::string &id,
                           const std::string &count) const {
    auto response = get_req<media::SimilarSongs2>(
        "getSimilarSongs2", {{"id", id}, {"count", count}}, "similarSongs2");

    // extract data
    if (response)
        return check(response.value());
    else
        return std::unexpected(response.error());
}

// Album/song lists
// Returns a list of random, newest, highest rated etc. albums.
std::expected<album::AlbumList2, server::Error>
OSClient::getAlbumList2(const std::string &type, const std::string &size,
                        const std::string &offset, const std::string &fromYear,
                        const std::string &toYear,
                        const std::string &genre) const {

    // make parameters
    std::map<std::string, std::string> params{
        {"type", type},         {"size", size},     {"offset", size},
        {"fromYear", fromYear}, {"toYear", toYear}, {"genre", genre}};

    // get response
    auto response =
        get_req<album::AlbumList2>("getAlbumList2", params, "albumList2");

    // extract data
    if (response) {
        return check(response.value());
    } else
        return std::unexpected(response.error());
}

//  Returns random songs matching the given criteria.
std::expected<media::RandomSongs, server::Error>
OSClient::getRandomSongs(const std::string &size, const std::string &genre,
                         const std::string &fromYear,
                         const std::string &toYear) const {
    // make params
    std::map<std::string, std::string> params{{"size", size},
                                              {"genre", genre},
                                              {"fromYear", fromYear},
                                              {"toYear", toYear}};

    auto response =
        get_req<media::RandomSongs>("getRandomSongs", params, "randomSongs");

    // extract data
    if (response)
        return check(response.value());
    else
        return std::unexpected(response.error());
}

// Returns what is currently being played by all users. Takes no extra
// parameters
std::expected<media::NowPlaying, server::Error>
OSClient::getNowPlaying() const {
    auto response =
        get_req<media::NowPlaying>("getNowPlaying", {}, "nowPlaying");

    // extract data
    if (response)
        return check(response.value());
    else
        return std::unexpected(response.error());
}

// private
/// helper for GET requests
template <class Data>
std::expected<server::SubsonicResponse<Data>, server::Error>
OSClient::get_req(const std::string &endpoint,
                  const std::map<std::string, std::string> &params,
                  const std::string &key) const {

    // basic request params required by every endpoint
    cpr::Parameters request_params{{"u", m_username},    {"t", m_token},
                                   {"s", m_salt},        {"v", API_VERSION},
                                   {"c", m_client_name}, {"f", "json"}};

    // add endpoint specific params to request_params
    for (auto const &param : params) {
        auto p = cpr::Parameter({param.first, param.second});
        request_params.Add(p);
    }

    cpr::Response r =
        cpr::Get(cpr::Url{m_server_url + endpoint}, request_params);

    // if the request is not successful
    if (r.status_code != 200)
        return std::unexpected(server::Error{
            static_cast<std::size_t>(r.status_code), r.error.message});

    // if the request is successful
    // (there may still be errors)
    else {
        json j = json::parse(r.text);

        auto response = j["subsonic-response"]
                            .template get<server::SubsonicResponse<Data>>();

        // extract data
        if (j["subsonic-response"].contains(key)) {
            response.data = j["subsonic-response"][key].template get<Data>();
            return response;
        } else if (response.status == "ok" ||
                   j["subsonic-response"].contains("error")) {
            return response;
        } else {
            return std::unexpected(server::Error{500, "unknown key"});
        }
    }
};

// check the response data
template <class Data>
std::expected<Data, server::Error>
OSClient::check(server::SubsonicResponse<Data> &r) const {
    if (r.status == "ok")
        return r.data;
    else
        return std::unexpected(r.error);
}

namespace uboat::artist {
// json parsers
// ArtistID3
void from_json(const nlohmann::json &j, ArtistID3 &a) {
    j.at("id").get_to(a.id);
    j.at("name").get_to(a.name);
    set_if_contains(j, "coverArt", a.coverArt);
    set_if_contains(j, "artistImageUrl", a.artistImageUrl);
    set_if_contains(j, "albumCount", a.albumCount);
    set_if_contains(j, "userRating", a.userRating);
    set_if_contains(j, "starred", a.starred);
    set_if_contains(j, "musicBrainzId", a.musicBrainzId);
    set_if_contains(j, "sortName", a.sortName);
    set_if_contains(j, "roles", a.roles);
}
// ArtistInfo2
void from_json(const nlohmann::json &j, ArtistInfo2 &a) {
    set_if_contains(j, "biography", a.biography);
    set_if_contains(j, "musicBrainzId", a.musicBrainzId);
    set_if_contains(j, "lastFmUrl", a.lastFmUrl);
    set_if_contains(j, "smallImageUrl", a.smallImageUrl);
    set_if_contains(j, "mediumImageUrl", a.mediumImageUrl);
    set_if_contains(j, "largeImageUrl", a.largeImageUrl);
    set_if_contains(j, "similarArtist", a.similarArtist);
}
// IndexID3
void from_json(const nlohmann::json &j, IndexID3 &i) {
    j.at("name").get_to(i.name);
    j.at("artist").get_to(i.artist);
}

// Artists
void from_json(const nlohmann::json &j, Artists &a) {
    j.at("ignoredArticles").get_to(a.ignoredArticles);
    set_if_contains(j, "index", a.index);
}
} // namespace uboat::artist

namespace uboat::misc {
// json parsers
// Genres
void from_json(const nlohmann::json &j, Genres &g) {
    set_if_contains(j, "genre", g.genre);
}

// ItemDate
void from_json(const nlohmann::json &j, ItemDate &i) {
    set_if_contains(j, "year", i.year);
    set_if_contains(j, "month", i.month);
    set_if_contains(j, "day", i.day);
}

// ReplayGain
void from_json(const nlohmann::json &j, ReplayGain &r) {
    set_if_contains(j, "trackGain", r.trackGain);
    set_if_contains(j, "albumGain", r.albumGain);
    set_if_contains(j, "trackPeak", r.trackPeak);
    set_if_contains(j, "albumPeak", r.albumPeak);
    set_if_contains(j, "baseGain", r.baseGain);
    set_if_contains(j, "fallbackGain", r.fallbackGain);
}
} // namespace uboat::misc

namespace uboat::media {
// json parser
// Child
void from_json(const nlohmann::json &j, Child &c) {
    j.at("id").get_to(c.id);

    set_if_contains(j, "parent", c.parent);

    j.at("isDir").get_to(c.isDir);
    j.at("title").get_to(c.title);

    set_if_contains(j, "album", c.album);
    set_if_contains(j, "artist", c.artist);
    set_if_contains(j, "track", c.track);
    set_if_contains(j, "year", c.year);
    set_if_contains(j, "genre", c.genre);
    set_if_contains(j, "coverArt", c.coverArt);
    set_if_contains(j, "size", c.size);
    set_if_contains(j, "contentType", c.contentType);
    set_if_contains(j, "suffix", c.suffix);
    set_if_contains(j, "transcodedContentType", c.transcodedContentType);
    set_if_contains(j, "transcodedSuffix", c.transcodedSuffix);
    set_if_contains(j, "duration", c.duration);
    set_if_contains(j, "bitRate", c.bitRate);
    set_if_contains(j, "bitDepth", c.bitDepth);
    set_if_contains(j, "samplingRate", c.samplingRate);
    set_if_contains(j, "channelCount", c.channelCount);
    set_if_contains(j, "path", c.path);
    set_if_contains(j, "isVideo", c.isVideo);
    set_if_contains(j, "userRating", c.userRating);
    set_if_contains(j, "averageRating", c.averageRating);
    set_if_contains(j, "playCount", c.playCount);
    set_if_contains(j, "discNumber", c.discNumber);
    set_if_contains(j, "created", c.created);
    set_if_contains(j, "starred", c.starred);
    set_if_contains(j, "albumId", c.albumId);
    set_if_contains(j, "artistId", c.artistId);
    set_if_contains(j, "type", c.type);
    set_if_contains(j, "mediaType", c.mediaType);
    set_if_contains(j, "bookmarkPosition", c.bookmarkPosition);
    set_if_contains(j, "originalWidth", c.originalWidth);
    set_if_contains(j, "originalHeight", c.originalHeight);
    set_if_contains(j, "played", c.played);
    set_if_contains(j, "bpm", c.bpm);
    set_if_contains(j, "comment", c.comment);
    set_if_contains(j, "sortName", c.sortName);
    set_if_contains(j, "musicBrainzId", c.musicBrainzId);
    set_if_contains(j, "genres", c.genres);
    set_if_contains(j, "artists", c.artists);
    set_if_contains(j, "displayArtist", c.displayArtist);
    set_if_contains(j, "albumArtists", c.albumArtists);
    set_if_contains(j, "displayAlbumArtist", c.displayAlbumArtist);
    set_if_contains(j, "replayGain", c.replayGain);
}

// NowPlayingEntry
void from_json(const nlohmann::json &j, NowPlayingEntry &n) {

    from_json(j, static_cast<Child &>(n));

    j.at("username").get_to(n.username);

    set_if_contains(j, "minutesAgo", n.minutesAgo);
    set_if_contains(j, "playerId", n.playerId);
    set_if_contains(j, "playerName", n.playerName);
}

// RandomSongs
void from_json(const nlohmann::json &j, RandomSongs &r) {
    set_if_contains(j, "song", r.song);
}

// NowPlaying
void from_json(const nlohmann::json &j, NowPlaying &n) {
    set_if_contains(j, "entry", n.entry);
}

// SimilarSongs2
void from_json(const nlohmann::json &j, SimilarSongs2 &s) {
    set_if_contains(j, "song", s.song);
}
} // namespace uboat::media

namespace uboat::album {
// json parsers
// AlbumID3
void from_json(const nlohmann::json &j, AlbumID3 &a) {
    j.at("id").get_to(a.id);
    j.at("name").get_to(a.name);

    set_if_contains(j, "artist", a.artist);
    set_if_contains(j, "artistId", a.artistId);
    set_if_contains(j, "coverArt", a.coverArt);

    j.at("songCount").get_to(a.songCount);
    j.at("duration").get_to(a.duration);

    set_if_contains(j, "playCount", a.playCount);

    j.at("created").get_to(a.created);

    set_if_contains(j, "starred", a.starred);
    set_if_contains(j, "year", a.year);
    set_if_contains(j, "played", a.played);
    set_if_contains(j, "userRating", a.userRating);
    set_if_contains(j, "recordLabels", a.recordLabels);
    set_if_contains(j, "musicBrainzId", a.musicBrainzId);
    set_if_contains(j, "genres", a.genres);
    set_if_contains(j, "artists", a.artists);
    set_if_contains(j, "displayArtist", a.displayArtist);
    set_if_contains(j, "releaseTypes", a.releaseTypes);
    set_if_contains(j, "moods", a.moods);
    set_if_contains(j, "sortName", a.sortName);
    set_if_contains(j, "originalReleaseDate", a.originalReleaseDate);
    set_if_contains(j, "rCeleaseDate", a.releaseDate);
    set_if_contains(j, "isCompilation", a.isCompilation);
    set_if_contains(j, "discTitles", a.discTitles);
}

// AlbumID3WithSongs
void from_json(const nlohmann::json &j, AlbumID3WithSongs &a) {
    from_json(j, static_cast<AlbumID3 &>(a));
    set_if_contains(j, "song", a.song);
}

// AlbumInfo
void from_json(const nlohmann::json &j, AlbumInfo &a) {
    set_if_contains(j, "notes", a.notes);
    set_if_contains(j, "musicBrainzId", a.musicBrainzId);
    set_if_contains(j, "lastFmUrl", a.lastFmUrl);
    set_if_contains(j, "smallImageUrl", a.smallImageUrl);
    set_if_contains(j, "mediumImageUrl", a.mediumImageUrl);
    set_if_contains(j, "largeImageUrl", a.largeImageUrl);
}

// AlbumList2
void from_json(const nlohmann::json &j, AlbumList2 &a) {
    set_if_contains(j, "album", a.album);
}
} // namespace uboat::album

namespace uboat::server {
// json parsers
// License
void from_json(const nlohmann::json &j, License &l) {
    j.at("valid").get_to(l.valid);

    set_if_contains(j, "email", l.email);
    set_if_contains(j, "licenseExpires", l.licenseExpires);
    set_if_contains(j, "trialExpires", l.trialExpires);
}

// SubsonicResponse

template <class Data>
void from_json(const nlohmann::json &j, SubsonicResponse<Data> &s) {
    j.at("status").get_to(s.status);
    j.at("version").get_to(s.version);
    j.at("type").get_to(s.type);
    j.at("serverVersion").get_to(s.serverVersion);
    j.at("openSubsonic").get_to(s.openSubsonic);

    set_if_contains(j, "error", s.error);
}
} // namespace uboat::server

namespace uboat {

void set_if_contains(const nlohmann::json &j, const std::string &key, auto &v) {
    if (j.contains(key))
        j.at(key).get_to(v);
}

} // namespace uboat
