add_library(cache_lib
    src/CacheManager.cpp
)


target_include_directories(cache_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(cache_lib
    PUBLIC
        spdlog::spdlog
)