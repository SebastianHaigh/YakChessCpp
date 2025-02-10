add_executable(MagicTests MagicTests.cpp)
target_link_libraries(MagicTests
                      PUBLIC
                       MagicBitboards
                       YakBitboard
                      PRIVATE
                       Catch2::Catch2WithMain)

target_include_directories(MagicTests PUBLIC /YakChessCpp/build/src/magic
                                             /YakChessCpp/src)

add_test(NAME MagicTests
         COMMAND MagicTests)

