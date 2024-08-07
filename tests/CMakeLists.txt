macro(add_uboat_test _TEST_NAME)
  add_executable(${_TEST_NAME}_tests ${_TEST_NAME}_tests.cpp)
  target_link_libraries(${_TEST_NAME}_tests PRIVATE ${CMAKE_PROJECT_NAME})
  add_test(NAME ${_TEST_NAME}_tests COMMAND ${_TEST_NAME}_tests)
endmacro()

add_uboat_test(system)
