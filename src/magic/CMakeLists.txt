add_subdirectory(tests)

add_executable(MagicGenerator MagicGenerator.cpp)
target_link_libraries(MagicGenerator PRIVATE YakTypes YakBitboard)

set(MAGIC_GENERATOR ${CMAKE_CURRENT_BINARY_DIR}/MagicGenerator)
set(GENERATED_HEADER ${CMAKE_CURRENT_BINARY_DIR}/GeneratedMagics.h)

add_custom_command(OUTPUT ${GENERATED_HEADER}
                   COMMAND ${MAGIC_GENERATOR} ${GENERATED_HEADER}
                   DEPENDS ${MAGIC_GENERATOR}
                   COMMENT "Generating GeneratedMagics.h")

add_custom_target(generate_magic_header DEPENDS ${GENERATED_HEADER})

add_library(GeneratedMagic INTERFACE ${GENERATED_HEADER})
add_dependencies(GeneratedMagic generate_magic_header)

# Include the generated directory so other targets can find magics.h
target_include_directories(GeneratedMagic INTERFACE ${CMAKE_CURRENT_BINARY_DIR})

add_library(MagicBitboards INTERFACE MagicBitboards.hpp)
add_dependencies(MagicBitboards generate_magic_header GeneratedMagic)
target_include_directories(MagicBitboards INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
