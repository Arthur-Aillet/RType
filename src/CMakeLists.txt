# add_executable(rtype main.cpp)

# target_link_libraries(rtype PUBLIC cevy)

# target_include_directories(rtype PUBLIC ../Cevy/src)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb")

add_executable(bunny bunny.cpp BunnyMesh.cpp)

target_link_libraries(bunny PUBLIC cevy)

get_cevy_engine_assets(bunny)
# copy_cevy_engine_assets_to(bunny ${CMAKE_SOURCE_DIR})

# target_include_directories(bunny PRIVATE ../Cevy/src)
