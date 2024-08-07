add_executable(uboat_example main.cpp)
target_link_libraries(uboat_example PRIVATE ${CMAKE_PROJECT_NAME} nlohmann_json::nlohmann_json)
