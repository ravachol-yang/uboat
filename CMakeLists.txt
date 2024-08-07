cmake_minimum_required(VERSION 3.10...3.30)

project(
  uboat
  VERSION 0.0.1
  DESCRIPTION ""
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(UBOAT_BUILD_TESTING "Build the testing tree." OFF)

# for testing
include(CTest)

# libraries nlohmann::json for json parsing
find_package(nlohmann_json 3.2.0 REQUIRED)

# openssl
find_package(OpenSSL REQUIRED)

# cpr for http(s) request
include(FetchContent)
FetchContent_Declare(
  cpr
  GIT_REPOSITORY https://github.com/libcpr/cpr.git
  GIT_TAG 1.10.5)
FetchContent_MakeAvailable(cpr)

include_directories(include)

add_subdirectory(examples)

set(SRC_LIST src/uboat.cpp)

# add directory for unit tests
if(UBOAT_BUILD_TESTING)
  message(STATUS "Building of tests is enabled.")
  enable_testing()
  add_subdirectory(tests)
endif()

add_library(${CMAKE_PROJECT_NAME} ${SRC_LIST})


target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json
                                                    cpr::cpr OpenSSL::Crypto)
