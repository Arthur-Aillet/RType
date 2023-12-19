file(GLOB TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")

add_executable(test-rtype ${TEST_SOURCES})

target_link_libraries(test-rtype PUBLIC criterion)
