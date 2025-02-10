add_executable(MagicTests MagicTests.cpp)
target_link_libraries(MagicTests
                      PUBLIC
                       MagicBitboards
                       YakBitboard
                      PRIVATE
                       Catch2::Catch2WithMain)

target_include_directories(MagicTests PUBLIC $<TARGET_PROPERTY:GeneratedMagic,INTERFACE_INCLUDE_DIRECTORIES>)


add_test(NAME MagicTests
         COMMAND MagicTests)

add_executable(AttackMapTests AttackMapTests.cpp)
target_link_libraries(AttackMapTests
                      PUBLIC
                        MagicBitboards
                        YakTypes
                        YakBitboard
                      PRIVATE
                        Catch2::Catch2WithMain)

add_test(NAME AttackMapTests
         COMMAND AttackMapTests)

