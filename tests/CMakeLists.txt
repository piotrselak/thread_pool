include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/doctest ${CMAKE_CURRENT_SOURCE_DIR}/../include/thread_pool)

add_executable(tests tests.cpp)

target_link_libraries(tests PRIVATE thread_pool)
add_executable(lib_tests tests.cpp)
