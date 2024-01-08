add_executable(rtype main.cpp)

target_link_libraries(rtype PUBLIC cevy)

target_include_directories(rtype PUBLIC ../Cevy/src)
