#include "uboat/uboat.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "common.h"

TEST_SUITE("System") {
    auto client = uboat::OSClient(TEST_SERVER, TEST_USERNAME, TEST_PASSWORD,
                                  TEST_CLIENT_NAME);
    auto client_wrong_pass =
        uboat::OSClient(TEST_SERVER, TEST_USERNAME, "wrong", "wrong client");

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

    TEST_CASE("server not found") {
        auto client_wrong = uboat::OSClient("127.0.0.666", TEST_USERNAME,
                                            TEST_PASSWORD, TEST_CLIENT_NAME);
        auto result = client_wrong.ping();
        CHECK_FALSE(result.has_value());
        CHECK_EQ(result.error().code, 0);
        CAPTURE(result.error().message);
    }

    TEST_CASE("wrong credentials") {
        auto result = client_wrong_pass.authenticate();
        CHECK_FALSE(result.has_value());
        CHECK_EQ(result.error().code, 40);
    }

    TEST_CASE("get license successful") {
        auto result = client.getLicense();
        CHECK(result.has_value());
        CHECK(result.value().valid);
    }

    TEST_CASE("get license wrong credentials") {
        auto result = client_wrong_pass.getLicense();
        CHECK_FALSE(result.has_value());
        CHECK_EQ(result.error().code, 40);
    }
}
