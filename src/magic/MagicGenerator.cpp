#include "MagicNumberGeneration.h"
#include <types.h>

#include <iomanip>
#include <sstream>
#include <iostream>
#include <fstream>

std::string formatMagicArray(const std::array<yak::Bitboard, 64>& magics)
{
  std::stringstream ss;

  yak::Square square{ yak::A1 };

  for (const auto& magic : magics)
  {
    std::ostringstream hexStream;
    hexStream << "0x" << std::hex << magic << ",";

    std::ostringstream commentStream;
    commentStream << "// Magic for square " << yak::toAlgebraic(square);
    square = static_cast<yak::Square>(square + 1);

    ss << "  " << std::setw(20) << std::left << hexStream.str()
      << "  " << commentStream.str() << "\n";
  }

  return ss.str();
}


int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <output_file>\n";
    return 1;
  }

  std::ofstream outputFile(argv[1]);
  if (!outputFile)
  {
    std::cerr << "Error: Could not open file for writing!\n";
    return 1;
  }

  std::stringstream ss;

  ss << "#include <array>\n\n";
  ss << "namespace yak::magic {\n";

  ss << "static constexpr std::array<Bitboard, 64> ROOK_MAGICS = { \n";

  auto rookMagics = yak::magic::generation::findAllMagics<yak::PieceType::ROOK>();
  ss << formatMagicArray(rookMagics);

  ss << "};\n\n";

  ss << "static constexpr std::array<Bitboard, 64> BISHOP_MAGICS = { \n";

  auto bishopMagics = yak::magic::generation::findAllMagics<yak::PieceType::BISHOP>();
  ss << formatMagicArray(bishopMagics);

  ss << "};\n\n";

  ss << "} // namespace yak::magic\n";
  ss << "\n";

  outputFile << ss.str();
  outputFile.close();

  return 0;
}

