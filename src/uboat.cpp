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
    // TODO
    m_salt = "test";

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

template <class Data>
std::expected<Data, server::Error>
OSClient::check(server::SubsonicResponse<Data> &r) const {
    if (r.status == "ok")
        return r.data;
    else
        return std::unexpected(r.error);
}
