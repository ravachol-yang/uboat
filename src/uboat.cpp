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
std::expected<album::AlbumID3WithSongs, server::Error>
OSClient::getAlbum(const std::string &id) const {
    auto response =
        get_req<album::AlbumID3WithSongs>("getAlbum", {{"id", id}}, "albums");
    return {};
}

// Album/song lists
std::expected<album::AlbumList2, server::Error>
OSClient::getAlbumList2(const std::string &type, const size_t &size,
                        const size_t &offset, const size_t &fromYear,
                        const size_t &toYear, const std::string &genre) const {

    // make parameters
    std::map<std::string, std::string> params{
        {"type", type},
        {"size", std::to_string(size)},
        {"offset", std::to_string(offset)}};

    // add optional params
    if (type == "byYear")
        params.insert({{"fromYear", std::to_string(fromYear)},
                       {"toYear", std::to_string(toYear)}});
    else if (type == "byGenre")
        params.insert({{"genre", genre}});

    // get response
    auto response =
        get_req<album::AlbumList2>("getAlbumList2", params, "albumList2");

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

namespace uboat::album {
// json parsers
// AlbumID3
void from_json(const nlohmann::json &j, AlbumID3 &a) {
    j.at("id").get_to(a.id);
    j.at("name").get_to(a.name);

    if (j.contains("artist"))
        j.at("artist").get_to(a.artist);

    if (j.contains("artistId"))
        j.at("artistId").get_to(a.artistId);

    if (j.contains("coverArt"))
        j.at("coverArt").get_to(a.coverArt);

    j.at("songCount").get_to(a.songCount);
    j.at("duration").get_to(a.duration);

    if (j.contains("playCount"))
        j.at("playCount").get_to(a.playCount);

    j.at("created").get_to(a.created);

    if (j.contains("starred"))
        j.at("starred").get_to(a.starred);

    if (j.contains("year"))
        j.at("year").get_to(a.year);

    if (j.contains("played"))
        j.at("played").get_to(a.played);

    if (j.contains("userRating"))
        j.at("userRating").get_to(a.userRating);

    if (j.contains("recordLabels"))
        j.at("recordLabels").get_to(a.recordLabels);

    if (j.contains("musicBrainzId"))
        j.at("musicBrainzId").get_to(a.musicBrainzId);

    if (j.contains("genres"))
        j.at("genres").get_to(a.genres);

    if (j.contains("artists"))
        j.at("artists").get_to(a.artists);

    if (j.contains("displayArtist"))
        j.at("displayArtist").get_to(a.displayArtist);

    if (j.contains("releaseTypes"))
        j.at("releaseTypes").get_to(a.releaseTypes);

    if (j.contains("moods"))
        j.at("moods").get_to(a.moods);

    if (j.contains("sortNames"))
        j.at("sortNames").get_to(a.sortName);

    if (j.contains("originalReleaseDate"))
        j.at("originalReleaseDate").get_to(a.originalReleaseDate);

    if (j.contains("releaseDate"))
        j.at("releaseDate").get_to(a.releaseDate);

    if (j.contains("isCompilation"))
        j.at("isCompilation").get_to(a.isCompilation);

    if (j.contains("discTitles"))
        j.at("discTitles").get_to(a.discTitles);
}

// AlbumList2
void from_json(const nlohmann::json &j, AlbumList2 &a) {
    if (j.contains("album"))
        j.at("album").get_to(a.album);
}
} // namespace uboat::album

namespace uboat::server {
// json parsers
// License
void from_json(const nlohmann::json &j, License &l) {
    j.at("valid").get_to(l.valid);
    if (j.contains("email"))
        j.at("email").get_to(l.email);
    if (j.contains("licenseExpires"))
        j.at("licenseExpires").get_to(l.licenseExpires);
    if (j.contains("trialExpires"))
        j.at("trialExpires").get_to(l.trialExpires);
}

// SubsonicResponse

template <class Data>
void from_json(const nlohmann::json &j, SubsonicResponse<Data> &s) {
    j.at("status").get_to(s.status);
    j.at("version").get_to(s.version);
    j.at("type").get_to(s.type);
    j.at("serverVersion").get_to(s.serverVersion);
    j.at("openSubsonic").get_to(s.openSubsonic);
    if (j.contains("error"))
        j.at("error").get_to(s.error);
}
} // namespace uboat::server
