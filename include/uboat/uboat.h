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
#include <map>
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
/// https://opensubsonic.netlify.app/docs/responses/indexes/
struct Indexes {
    std::string ignoredArticles;
    std::vector<IndexID3> index;
};

// Define types for json parsing
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ArtistID3, id, name, coverArt,
                                   artistImageUrl, albumCount, userRating,
                                   starred, musicBrainzId, sortName, roles)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ArtistInfo2, biography, musicBrainzId,
                                   lastFmUrl, smallImageUrl, mediumImageUrl,
                                   largeImageUrl, similarArtist)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(IndexID3, name, artist)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Indexes, ignoredArticles, index)
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

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Genre, value, songCount, albumCount)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RecordLabel, name)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ItemGenre, name)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ItemDate, year, month, day)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DiscTitle, disc, title)
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
    // replayGain
};

// Define types for json parsing
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    Child, id, parent, isDir, title, album, artist, track, year, genre,
    coverArt, size, contentType, suffix, transcodedContentType,
    transcodedSuffix, duration, bitRate, bitDepth, samplingRate, channelCount,
    path, isVideo, userRating, averageRating, playCount, discNumber, created,
    starred, albumId, artistId, type, mediaType, bookmarkPosition,
    originalWidth, originalHeight, played, bpm, comment, sortName,
    musicBrainzId, genres, artists, displayArtist, albumArtists,
    displayAlbumArtist)
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

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AlbumID3WithSongs, id, name, artist,
                                   artistId, coverArt, songCount, duration,
                                   playCount, created, starred, year, genre,
                                   played, userRating, recordLabels,
                                   musicBrainzId, genres, artists,
                                   displayArtist, releaseTypes, moods, sortName,
                                   originalReleaseDate, releaseDate,
                                   isCompilation, discTitles, song)

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

    /// Returns details for an album, including a list of songs. This method
    /// organizes music according to ID3 tags.
    /// https://opensubsonic.netlify.app/docs/endpoints/getalbum/
    /// \param id the album id
    /// \return returns an ablum and songs on success and error on failure
    std::expected<album::AlbumID3WithSongs, server::Error>
    getAlbum(const std::string &id) const;

    // Album/song lists

    /// Returns a list of random, newest, highest rated etc. albums.
    /// Similar to getAlbumList, but organizes music according to ID3 tags.
    /// https://opensubsonic.netlify.app/docs/endpoints/getalbumlist2/
    /// \param
    std::expected<album::AlbumList2, server::Error>
    getAlbumList2(const std::string &type, const size_t &size = 10,
                  const size_t &offset = 0, const size_t &fromYear = 0,
                  const size_t &toYear = 0,
                  const std::string &genre = "") const;

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
