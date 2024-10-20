//===-- uboat/uboat.h - models and OSClient class ------------*- C++-*---===//
//
// SPDX-License-Identifier: GPL-3.0-only
//
//===-----------------------------------------------------------------------------===//
///
/// \uboat.h
/// This file contains the declaration of the OSClient class
/// with OpenSubsonic API endpoint wrapper functions, see:
/// https://opensubsonic.netlify.app/docs/endpoints/
/// and with entity models, see:
/// https://opensubsonic.netlify.app/docs/responses/
///
//===----------------------------------------------------------------------------===//
//

#ifndef UBOAT_H
#define UBOAT_H

#include <cstddef>
#include <expected>
#include <nlohmann/detail/macro_scope.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace uboat {

static constexpr std::string PROJECT_NAME = "uboat";
static constexpr std::string VERSION = "0.0.1";
static constexpr std::string API_VERSION =
    "1.16.1"; /* supported OpenSubsonic API version  */

namespace artist {

/// An artist from ID3 tags.
/// https://opensubsonic.netlify.app/docs/responses/artistid3/
struct ArtistID3 {
    std::string id;
    std::string name;
    std::string coverArt;
    std::string artistImageUrl;
    std::size_t albumCount;
    std::size_t userRating;
    std::string starred;
    std::string musicBrainzId;
    std::string sortName;
    std::vector<std::string> roles;
};

/// Artist info.
/// https://opensubsonic.netlify.app/docs/responses/artistinfo2/
struct ArtistInfo2 {
    std::string biography;
    std::string musicBrainzId;
    std::string lastFmUrl;
    std::string smallImageUrl;
    std::string mediumImageUrl;
    std::string largeImageUrl;
    std::vector<ArtistID3> similarArtist;
};

/// An indexed artist list.
/// https://opensubsonic.netlify.app/docs/responses/indexid3/
struct IndexID3 {
    std::string name;
    std::vector<ArtistID3> artist;
};

/// Artist list.
/// https://opensubsonic.netlify.app/docs/responses/artists/
struct Artists {
    std::string ignoredArticles;
    std::vector<IndexID3> index;
};

/// Artist list.
/// https://opensubsonic.netlify.app/docs/responses/indexes/
struct Indexes {
    std::string ignoredArticles;
    std::vector<IndexID3> index;
};

} // namespace artist

namespace misc {

/// https://opensubsonic.netlify.app/docs/responses/genre/
struct Genre {
    std::string value;
    std::size_t songCount;
    std::size_t albumCount;
};

struct RecordLabel {
    std::string name;
};

struct ItemGenre {
    std::string name;
};

struct ItemDate {
    std::size_t year;
    std::size_t month;
    std::size_t day;
};

struct DiscTitle {
    std::size_t disc;
    std::string title;
};

struct ReplayGain {
    size_t trackGain;
    size_t albumGain;
    size_t trackPeak;
    size_t albumPeak;
    size_t baseGain;
    size_t fallbackGain;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Genre, value, songCount, albumCount)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RecordLabel, name)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ItemGenre, name)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DiscTitle, disc, title)

// json parsers
// ItemDate
void from_json(const nlohmann::json &j, ItemDate &i);

// ReplayGain
void from_json(const nlohmann::json &j, ReplayGain &r);

} // namespace misc

namespace media {

/// A media
/// https://opensubsonic.netlify.app/docs/responses/child/
struct Child {
    std::string id;
    std::string parent;
    bool isDir;
    std::string title;
    std::string album;
    std::string artist;
    std::size_t track;
    std::size_t year;
    std::string genre;
    std::string coverArt;
    std::size_t size;
    std::string contentType;
    std::string suffix;
    std::string transcodedContentType;
    std::string transcodedSuffix;
    std::size_t duration;
    std::size_t bitRate;
    std::size_t bitDepth;
    std::size_t samplingRate;
    std::size_t channelCount;
    std::string path;
    bool isVideo;
    std::size_t userRating;
    std::size_t averageRating;
    std::size_t playCount;
    std::size_t discNumber;
    std::string created;
    std::string starred;
    std::string albumId;
    std::string artistId;
    std::string type;
    std::string mediaType;
    std::size_t bookmarkPosition;
    std::size_t originalWidth;
    std::size_t originalHeight;
    std::string played;
    std::size_t bpm;
    std::string comment;
    std::string sortName;
    std::string musicBrainzId;
    std::vector<misc::ItemGenre> genres;
    std::vector<artist::ArtistID3> artists;
    std::string displayArtist;
    std::vector<artist::ArtistID3> albumArtists;
    std::string displayAlbumArtist;
    // contributors
    // displayComposer
    // moods
    misc::ReplayGain replayGain;
};

/// NowPlayingEntry extends Child (See Child documentation for the corresponding
/// fields) https://opensubsonic.netlify.app/docs/responses/nowplayingentry/
struct NowPlayingEntry : public Child {
    std::string username;
    size_t minutesAgo;
    size_t playerId;
    std::string playerName;
};

/// RandomSongs list.
/// https://opensubsonic.netlify.app/docs/responses/randomsongs/
struct RandomSongs {
    std::vector<Child> song;
};

/// nowPlaying
/// https://opensubsonic.netlify.app/docs/responses/nowplaying/
struct NowPlaying {
    std::vector<NowPlayingEntry> entry;
};

// json parser
// Child
void from_json(const nlohmann::json &j, Child &c);

// NowPlayingEntry
void from_json(const nlohmann::json &j, NowPlayingEntry &n);

// RandomSongs
void from_json(const nlohmann::json &j, RandomSongs &r);

// NowPlaying
void from_json(const nlohmann::json &j, NowPlaying &n);

} // namespace media

namespace album {

/// An album from ID3 tags.
/// https://opensubsonic.netlify.app/docs/responses/albumid3/
struct AlbumID3 {
    std::string id;
    std::string name;
    std::string artist;
    std::string artistId;
    std::string coverArt;
    std::size_t songCount;
    std::size_t duration;
    std::size_t playCount;
    std::string created;
    std::string starred;
    std::size_t year;
    std::string genre;
    std::string played;
    std::size_t userRating;
    std::vector<misc::RecordLabel> recordLabels;
    std::string musicBrainzId;
    std::vector<misc::ItemGenre> genres;
    std::vector<artist::ArtistID3> artists;
    std::string displayArtist;
    std::vector<std::string> releaseTypes;
    std::vector<std::string> moods;
    std::string sortName;
    misc::ItemDate originalReleaseDate;
    misc::ItemDate releaseDate;
    bool isCompilation;
    std::vector<misc::DiscTitle> discTitles;
};
struct AlbumID3WithSongs : AlbumID3 {
    std::vector<media::Child> song;
};

struct AlbumList2 {
    std::vector<AlbumID3> album;
};

// json parsers
// AlbumID3
void from_json(const nlohmann::json &j, AlbumID3 &a);

// AlbumID3WithSongs
void from_json(const nlohmann::json &j, AlbumID3WithSongs &a);

// AlbumList2
void from_json(const nlohmann::json &j, AlbumList2 &a);
} // namespace album

namespace server {

/// Error
/// https://opensubsonic.netlify.app/docs/responses/error/
struct Error {
    std::size_t code;
    std::string message;
};

struct License {
    bool valid;
    std::string email;
    std::string licenseExpires;
    std::string trialExpires;
};

template <class Data> struct SubsonicResponse {
    std::string status;
    std::string version;
    std::string type;
    std::string serverVersion;
    bool openSubsonic;
    Error error;
    Data data; /* to store response data */
};

// Define types for json parsing
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Error, code, message)

// json parsers
// License
void from_json(const nlohmann::json &j, License &l);

// SubsonicResponse
template <class Data>
void to_json(nlohmann::json &j, const SubsonicResponse<Data> &s) {
    j = nlohmann::json{{"status", s.status},
                       {"version", s.version},
                       {"type", s.type},
                       {"serverVersion", s.serverVersion},
                       {"openSubsonic", s.openSubsonic}};
}

template <class Data>
void from_json(const nlohmann::json &j, SubsonicResponse<Data> &s);
} // namespace server

// helper for optional fields
void set_if_contains(const nlohmann::json &j, const std::string &key, auto &v);

/// OpenSubsonic Client
class OSClient {
public:
    OSClient(const std::string &server_url, const std::string &username,
             const std::string &password, const std::string &client_name);
    ~OSClient() = default;

    /// Generate MD5 token and try to ping() the server
    /// **Must be called before any other endpoints**
    /// \return returns a SubsonicResponse on success
    std::expected<server::SubsonicResponse<server::Error>, server::Error>
    authenticate();

    // API Endpoints:

    // System

    /// Used to (only) test connectivity with the server.
    /// https://opensubsonic.netlify.app/docs/endpoints/ping/
    /// \return returns a SubsonicResponse on success
    std::expected<server::SubsonicResponse<server::Error>, server::Error>
    ping() const;

    /// Get details about the software license.
    /// https://opensubsonic.netlify.app/docs/endpoints/getlicense/
    /// \return A subsonic-response element with a nested license element on
    /// success.
    std::expected<server::License, server::Error> getLicense() const;

    // Browsing

    /// Similar to getIndexes, but organizes music according to ID3 tags.
    /// https://opensubsonic.netlify.app/docs/endpoints/getartists/
    /// \return Artists or Error
    std::expected<artist::Artists, server::Error> getArtists() const;

    /// Returns details for an album, including a list of songs. This method
    /// organizes music according to ID3 tags.
    /// https://opensubsonic.netlify.app/docs/endpoints/getalbum/
    ///
    /// \param id the album id
    /// \return returns an ablum and songs on success and error on failure
    std::expected<album::AlbumID3WithSongs, server::Error>
    getAlbum(const std::string &id) const;

    // Album/song lists

    /// Returns a list of random, newest, highest rated etc. albums.
    /// Similar to getAlbumList, but organizes music according to ID3 tags.
    /// https://opensubsonic.netlify.app/docs/endpoints/getalbumlist2/
    ///
    /// \param type The list type. Must be one of the following: random, newest,
    /// highest, frequent, recent. Since 1.8.0 you can also use
    /// alphabeticalByName or alphabeticalByArtist to page through all albums
    /// alphabetically, and starred to retrieve starred albums. Since 1.10.1 you
    /// can use byYear and byGenre to list albums in a given year range or
    /// genre.
    ///
    /// \param size The number of albums to return. Max 500.
    /// \param offset The list offset. Useful if you for example want to page
    /// through the list of newest albums.
    ///
    /// \param fromYear The first year in the range.
    /// If fromYear > toYear a reverse chronological list is returned.
    ///
    /// \param toYear The last year in the range.
    /// \param genre The name of the genre, e.g., “Rock”.
    /// \return AlbumList2 or Error
    std::expected<album::AlbumList2, server::Error> getAlbumList2(
        const std::string &type, const std::string &size = "",
        const std::string &offset = "", const std::string &fromYear = "",
        const std::string &toYear = "", const std::string &genre = "") const;

    /// Returns random songs matching the given criteria.
    /// https://opensubsonic.netlify.app/docs/endpoints/getrandomsongs/
    ///
    /// \param size The maximum number of songs to return. Max 500.
    /// \param genre Only returns songs belonging to this genre.
    /// \param fromYear Only return songs published after or in this year.
    /// \param toYear Only return songs published before or in this year.
    /// \return RandomSongs or Error
    std::expected<media::RandomSongs, server::Error>
    getRandomSongs(const std::string &size = "", const std::string &genre = "",
                   const std::string &fromYear = "",
                   const std::string &toYear = "") const;

    /// Returns what is currently being played by all users. Takes no extra
    /// parameters
    /// https://opensubsonic.netlify.app/docs/endpoints/getnowplaying/
    ///
    /// \return Nowplaying or Error
    std::expected<media::NowPlaying, server::Error> getNowPlaying() const;

private:
    // client information:
    std::string m_server_url; /* url of the server, without trailing "/" */
    std::string m_username;   /* username for requests */
    std::string m_password;   /* the user's password, will be encrypted */
    std::string m_token;      /* MD5 token generated from password and salt */
    std::string m_salt;       /* random salt */

    // the name of the client using this library ,will be appended by
    // "uboat-{version}" to be the parameter "c" in requests
    std::string m_client_name;

    /// helper for GET requests
    /// \param endpoint
    /// \param params the request parameters
    template <class Data>
    std::expected<server::SubsonicResponse<Data>, server::Error>
    get_req(const std::string &endpoint,
            const std::map<std::string, std::string> &params,
            const std::string &key) const;

    /// check the response data
    /// \param r response
    template <class Data>
    std::expected<Data, server::Error>
    check(server::SubsonicResponse<Data> &r) const;
};
} // namespace uboat

#endif /* UBOAT_H */
