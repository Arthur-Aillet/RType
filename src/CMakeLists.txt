# add_executable(rtype main.cpp)

# target_link_libraries(rtype PUBLIC cevy)

# target_include_directories(rtype PUBLIC ../Cevy/src)

add_executable(bunny bunny.cpp Bunny.cpp)

target_link_libraries(bunny PUBLIC cevy)

# target_include_directories(bunny PRIVATE ../Cevy/src)
