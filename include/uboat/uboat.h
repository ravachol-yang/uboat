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

struct Genres {
    std::vector<Genre> genre;
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

/// SimilarSongs2 list.
/// https://opensubsonic.netlify.app/docs/responses/similarsongs2/
struct SimilarSongs2 {
    std::vector<Child> song;
};

/// TopSongs list.
/// https://opensubsonic.netlify.app/docs/responses/topsongs/
struct TopSongs {
    std::vector<Child> song;
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

/// Album info
/// https://opensubsonic.netlify.app/docs/responses/albuminfo/
struct AlbumInfo {
    std::string notes;
    std::string musicBrainzId;
    std::string lastFmUrl;
    std::string smallImageUrl;
    std::string mediumImageUrl;
    std::string largeImageUrl;
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

namespace playlist {
/// https://opensubsonic.netlify.app/docs/responses/playlist/
struct Playlist {
    std::string id;
    std::string name;
    std::string comment;
    std::string owner;
    bool isPublic;
    std::size_t songCount;
    std::size_t duration;
    std::string created;
    std::string changed;
    std::string coverArt;
    std::vector<std::string> allowedUser;
};
/// https://opensubsonic.netlify.app/docs/responses/playlists/
struct Playlists {
    std::vector<Playlist> playlist;
};
/// https://opensubsonic.netlify.app/docs/responses/playlistwithsongs/
struct PlaylistWithSongs : public Playlist {
    std::vector<media::Child> entry;
};
} // namespace playlist

namespace search {
// search3 result.
// https://opensubsonic.netlify.app/docs/responses/searchresult3/
struct SearchResult3 {
    std::vector<artist::ArtistID3> artist;
    std::vector<album::AlbumID3> album;
    std::vector<media::Child> song;
};
} // namespace search

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

    /// Returns all genres
    /// https://opensubsonic.netlify.app/docs/endpoints/getgenres/
    /// \return A subsonic-response element with a nested genres element on
    /// success.
    std::expected<misc::Genres, server::Error> getGenres() const;

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

    /// Returns artist info.
    /// Similar to getArtistInfo, but organizes music according to ID3 tags.
    /// https://opensubsonic.netlify.app/docs/endpoints/getartistinfo2/
    ///
    /// \param id The artist, album or song ID.
    /// \param count Max number of similar artists to return.
    /// \param includeNotPresent Whether to return artists that are not present
    /// in the media library.
    /// \return A subsonic-response element with a nested artistInfo2 element on
    /// success.
    std::expected<artist::ArtistInfo2, server::Error>
    getArtistInfo2(const std::string &id, const std::string &count = "",
                   const std::string &includeNotPresent = "false") const;

    /// Returns album info.
    /// Similar to getAlbumInfo, but organizes music according to ID3 tags.
    /// https://opensubsonic.netlify.app/docs/endpoints/getalbuminfo2/
    ///
    /// \param id The album ID or song ID.
    /// \return A subsonic-response element with a nested albumInfo element on
    /// success.
    std::expected<album::AlbumInfo, server::Error>
    getAlbumInfo2(const std::string &id) const;

    /// Returns a random collection of songs from the given artist and similar
    /// artists. Similar to getSimilarSongs, but organizes music according to
    /// ID3 tags.
    /// https://opensubsonic.netlify.app/docs/endpoints/getsimilarsongs2/
    ///
    /// \param id The artist ID.
    /// \param count Max number of songs to return.
    ///
    /// \return A subsonic-response element with a nested similarSongs2 element
    /// on success.
    std::expected<media::SimilarSongs2, server::Error>
    getSimilarSongs2(const std::string &id,
                     const std::string &count = "") const;

    /// Returns top songs for the given artist.
    /// Returns top songs for the given artist, using data from last.fm
    /// https://opensubsonic.netlify.app/docs/endpoints/gettopsongs/
    ///
    /// \param artist The artist name.
    /// \param Max number of songs to return
    ///
    /// \return A subsonic-response element with a nested topSongs element on
    /// success.
    std::expected<media::TopSongs, server::Error>
    getTopSongs(const std::string &artist, const std::string &count = "") const;

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

    // Searching

    /// Returns albums, artists and songs matching the given search criteria.
    /// Supports paging through the result. Music is organized according to ID3
    /// tags
    /// https://opensubsonic.netlify.app/docs/endpoints/search3/
    ///
    /// \param query Search query. Returns all on empty query
    /// \param artistCount Maximum number of artists to return.
    /// \param artistOffset Search result offset for artists. Used for paging.
    /// \param albumCount Maximum number of albums to return.
    /// \param albumOffset Search result offset for albums. Used for paging.
    /// \param SongCount Maximum number of songs to return.
    /// \param SongOffset Search result offset for songs. Used for paging.
    /// \param musicFolderId Only return results from music folder with the
    /// given ID.
    ///
    /// \return A subsonic-response with a nested searchResult3 element on
    /// success.
    std::expected<search::SearchResult3, server::Error> search3(
        const std::string &query, const std::string &artistCount = "",
        const std::string &artistOffset = "",
        const std::string &albumCount = "", const std::string &albumOffset = "",
        const std::string &songCount = "", const std::string &songOffset = "",
        const std::string &musicFolderId = "") const;

    // Playlists

    /// Returns all playlists a user is allowed to play.
    /// https://opensubsonic.netlify.app/docs/endpoints/getplaylists/
    ///
    /// \param username If specified, return playlists for this user rather than
    /// for the authenticated user. The authenticated user must have admin role
    /// if this parameter is used.
    ///
    /// \return playlists
    std::expected<playlist::Playlists, server::Error>
    getPlaylists(const std::string &username = "") const;

    /// Returns a listing of files in a saved playlist.
    /// https://opensubsonic.netlify.app/docs/endpoints/getplaylist/
    ///
    /// \param id ID of the playlist to return, as obtained by getPlaylists.
    ///
    /// \return playlist with songs
    std::expected<playlist::PlaylistWithSongs, server::Error>
    getPlaylist(const std::string &id) const;

    /// Creates (or updates) a playlist.
    /// https://opensubsonic.netlify.app/docs/endpoints/createplaylist/
    ///
    /// \param playlistId The playlist ID.required if updating
    /// \param name The human-readable name of the playlist.required id creating
    /// \param songId ID of a song in the playlist.
    ///
    /// \return playlist with songs
    std::expected<playlist::PlaylistWithSongs, server::Error>
    createPlaylist(const std::string &playlistId, const std::string &name,
                   const std::vector<std::string> &songId = {}) const;

    /// Updates a playlist. Only the owner of a playlist is allowed to update
    /// it.
    /// https://opensubsonic.netlify.app/docs/endpoints/updateplaylist/
    ///
    /// \param playlistId
    /// \param name
    /// \param comment
    /// \param isPublic
    /// \param songIdToAdd allow multiple
    /// \param songIndexToRemove allow multiple
    ///
    /// \return empty response
    std::expected<server::SubsonicResponse<server::Error>, server::Error>
    updatePlaylist(
        const std::string &playlistId, const std::string &name = "",
        const std::string &comment = "", const std::string &isPublic = "",
        const std::vector<std::string> &songIdToAdd = {},
        const std::vector<std::string> &songIndexToRemove = {}) const;

    /// Deletes a saved playlist.
    /// https://opensubsonic.netlify.app/docs/endpoints/deleteplaylist/
    ///
    /// \param id ID of the playlist to delete, as obtained by getPlaylists.
    /// \return empty response
    std::expected<server::SubsonicResponse<server::Error>, server::Error>
    deletePlaylist(const std::string &id) const;

    // Media annotation

    /// Attaches a star to a song, album or artist.
    /// https://opensubsonic.netlify.app/docs/endpoints/star/
    ///
    /// \param id The ID of the file (song) or folder (album/artist) to
    /// star. Multiple parameters allowed. \param albumId The ID of an album
    /// to star. Use this rather than id if the client accesses the media
    /// collection according to ID3 tags rather than file structure.
    /// Multiple parameters allowed.
    ///
    /// \param artistId The ID of an artist to star
    ///
    /// \return An empty subsonic-response element on success.
    std::expected<server::SubsonicResponse<server::Error>, server::Error>
    star(const std::string &id = "", const std::string &albumId = "",
         const std::string &artistId = "") const;

    /// Removes a star to a song, album or artist
    /// https://opensubsonic.netlify.app/docs/endpoints/unstar/
    ///
    /// \param id
    /// \param albumId
    /// \param artistId
    ///
    /// \return An empty subsonic-response element on success.
    std::expected<server::SubsonicResponse<server::Error>, server::Error>
    unstar(const std::string &id = "", const std::string &albumId = "",
           const std::string &artistId = "") const;

    /// Sets the rating for a music file.
    /// https://opensubsonic.netlify.app/docs/endpoints/setrating/
    ///
    /// \param id A string which uniquely identifies the file (song) or folder
    /// (album/artist) to rate.
    /// \param rating The rating between 1 and 5 (inclusive), or 0 to remove the
    /// rating.
    ///
    /// \return An empty subsonic-response element on success.
    std::expected<server::SubsonicResponse<server::Error>, server::Error>
    setRating(const std::string &id, const std::string &rating) const;

    /// Registers the local playback of one or more media files.
    /// https://opensubsonic.netlify.app/docs/endpoints/scrobble/
    ///
    /// \param id A string which uniquely identifies the file to scrobble.
    /// \param time The time (in milliseconds since 1 Jan 1970) at which the
    /// song was listened to.
    /// \param submission Whether this is a “submission” or a “now playing”
    /// notification.
    ///
    /// \return An empty subsonic-response element on success.
    std::expected<server::SubsonicResponse<server::Error>, server::Error>
    scrobble(const std::string &id, const std::string &time = "",
             const std::string &submission = "") const;

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
            const std::multimap<std::string, std::string> &params,
            const std::string &key) const;

    /// check the response data
    /// \param r response
    template <class Data>
    std::expected<Data, server::Error>
    check(server::SubsonicResponse<Data> &r) const;
};
} // namespace uboat

#endif /* UBOAT_H */
