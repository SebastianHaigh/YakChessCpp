enable_testing()

add_subdirectory(test)
add_subdirectory(perft)
add_subdirectory(magic)

add_library(YakTypes types.cpp)
target_include_directories(YakTypes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(YakBitboard bitboard.cpp)
target_link_libraries(YakBitboard PUBLIC YakTypes)
target_include_directories(YakBitboard PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(yak board.cpp pieces.cpp GameState.cpp)
target_link_libraries(yak PUBLIC YakTypes YakBitboard GeneratedMagic MagicBitboards)
target_include_directories(yak PUBLIC $<TARGET_PROPERTY:MagicBitboards,INTERFACE_INCLUDE_DIRECTORIES> ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/magic)

