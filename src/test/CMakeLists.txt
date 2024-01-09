include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(tests attackmaps_test.cpp bitboard_test.cpp board_test.cpp pieces_test.cpp)
target_link_libraries(tests
                      PUBLIC
                        yak
                      PRIVATE
                        gtest_main)
include(GoogleTest)
gtest_discover_tests(tests)
