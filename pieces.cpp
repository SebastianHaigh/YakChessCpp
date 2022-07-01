#include "pieces.h"

namespace faster {
   
    void generate_pawn_moves(PieceColour colour, Move* move_list, int& move_counter, Bitboard pawn_positions, Bitboard empty_squares, Bitboard opponent_pieces) {

        Bitboard promotable = (colour == PieceColour::BLACK ? bitboard::RANK_2 : bitboard::RANK_7) & pawn_positions;
        Bitboard not_promotable = pawn_positions & ~promotable;

        /* NOT PROMOTITIONS              */ 
        /* ----------------------------- */ 
        Bitboard sources = (colour == PieceColour::BLACK ? bitboard::north_one(empty_squares) : bitboard::south_one(empty_squares)) & not_promotable;
        Bitboard targets = colour == PieceColour::BLACK ? bitboard::south_one(sources) : bitboard::north_one(sources);
        while (sources)
        {
            *move_list++ = make_quiet(bitboard::pop_LS1B(sources), bitboard::pop_LS1B(targets));
            move_counter++;
        }

        targets = ((colour == PieceColour::BLACK) ? bitboard::RANK_5 : bitboard::RANK_4) & empty_squares;
        sources = (colour == PieceColour::BLACK) ? bitboard::north_one(targets) : bitboard::south_one(targets) & empty_squares;
        sources = (colour == PieceColour::BLACK) ? bitboard::north_one(sources) : bitboard::south_one(sources) & not_promotable;
        while (sources)
        {
            *move_list++ = make_capture(bitboard::pop_LS1B(sources), bitboard::pop_LS1B(targets));
            move_counter++;
        }

        sources = (colour == PieceColour::BLACK ? bitboard::north_west_one(opponent_pieces) : bitboard::south_west_one(opponent_pieces)) & not_promotable;
        targets = colour == PieceColour::BLACK ? bitboard::south_east_one(sources) : bitboard::north_east_one(sources);
        while (sources)
        {
            *move_list++ = make_capture(bitboard::pop_LS1B(sources), bitboard::pop_LS1B(targets));
            move_counter++;
        }

        sources = (colour == PieceColour::BLACK ? bitboard::north_east_one(opponent_pieces) : bitboard::south_east_one(opponent_pieces)) & not_promotable;
        targets = colour == PieceColour::BLACK ? bitboard::south_west_one(sources) : bitboard::north_west_one(sources);
        while (sources)
        {
            *move_list++ = make_capture(bitboard::pop_LS1B(sources), bitboard::pop_LS1B(targets));
            move_counter++;
        }
            
        // PROMOTIONS
        sources = (colour == PieceColour::BLACK ? bitboard::north_one(empty_squares) : bitboard::south_one(empty_squares)) & promotable;
        targets = colour == PieceColour::BLACK ? bitboard::south_one(sources) : bitboard::north_one(sources);
        while (sources) {
            Square from = bitboard::pop_LS1B(sources);
            Square to = bitboard::pop_LS1B(targets);
            *move_list++ = make_quiet_promotion(from, to, PieceType::KNIGHT);
            *move_list++ = make_quiet_promotion(from, to, PieceType::BISHOP);
            *move_list++ = make_quiet_promotion(from, to, PieceType::ROOK);
            *move_list++ = make_quiet_promotion(from, to, PieceType::QUEEN);
            move_counter += 4;
        }

        sources = (colour == PieceColour::BLACK ? bitboard::north_west_one(opponent_pieces) : bitboard::south_west_one(opponent_pieces)) & promotable;
        targets = colour == PieceColour::BLACK ? bitboard::south_east_one(sources) : bitboard::north_east_one(sources);
        while (sources) {
            Square from = bitboard::pop_LS1B(sources);
            Square to = bitboard::pop_LS1B(targets);
            *move_list++ = make_capture_promotion(from, to, PieceType::KNIGHT);
            *move_list++ = make_capture_promotion(from, to, PieceType::BISHOP);
            *move_list++ = make_capture_promotion(from, to, PieceType::ROOK);
            *move_list++ = make_capture_promotion(from, to, PieceType::QUEEN);
            move_counter += 4;
        }

        sources = (colour == PieceColour::BLACK ? bitboard::north_east_one(empty_squares) : bitboard::south_east_one(empty_squares)) & promotable;
        targets = colour == PieceColour::BLACK ? bitboard::south_west_one(sources) : bitboard::north_west_one(sources);
        while (sources) {
            Square from = bitboard::pop_LS1B(sources);
            Square to = bitboard::pop_LS1B(targets);
            *move_list++ = make_capture_promotion(from, to, PieceType::KNIGHT);
            *move_list++ = make_capture_promotion(from, to, PieceType::BISHOP);
            *move_list++ = make_capture_promotion(from, to, PieceType::ROOK);
            *move_list++ = make_capture_promotion(from, to, PieceType::QUEEN);
            move_counter += 4;
        }
    }

    void jumping_piece_moves(attacks::AttackMap* atk_map, Move* move_list, int& move_counter, Bitboard piece_positions, Bitboard empty_squares, Bitboard opponent_pieces) {
        
        while (piece_positions) {
            Square from = pop_first(piece_positions);
            Bitboard quiet = atk_map->get(from) & empty_squares;
            while (quiet)
            {
                *move_list++ = make_quiet(from, bitboard::pop_LS1B(quiet));
                move_counter++;
            }
                

            Bitboard capture = atk_map->get(from) & opponent_pieces;
            while (capture)
            {
                *move_list++ = make_capture(from, bitboard::pop_LS1B(capture));
                move_counter++;
            }
                
        }
    }

    void generate_sliding_piece_moves(const std::vector<faster::Ray*>& atk_map, Move* move_list, int& move_counter, Bitboard piece_positions, Bitboard empty_squares, Bitboard opponent_pieces) {

        while (piece_positions) {
            Square from = pop_first(piece_positions);
            for (auto map : atk_map) {
                Bitboard quiet = map->get(from, ~empty_squares) & empty_squares;
                while (quiet)
                {
                    *move_list++ = make_quiet(from, bitboard::pop_LS1B(quiet));
                    move_counter++;
                }
                    

                Bitboard capture = map->get(from, ~empty_squares) & opponent_pieces;
                while (capture)
                {
                    *move_list++ = make_capture(from, bitboard::pop_LS1B(capture));
                    move_counter++;
                }
                    
            }
        }

    }

    void generate_sliding_piece_moves(const RookMap& atk_map, Move* move_list, int& move_counter, Bitboard piece_positions, Bitboard empty_squares, Bitboard opponent_pieces) {
        while (piece_positions) {
            Square from = pop_first(piece_positions);
            Bitboard atk_bb = atk_map.attacks(from, ~empty_squares);

            Bitboard quiet = atk_bb & empty_squares;
            while (quiet)
            {
                *move_list++ = make_quiet(from, bitboard::pop_LS1B(quiet));
                move_counter++;
            }

            Bitboard capture = atk_bb & opponent_pieces;
            while (capture)
            {
                *move_list++ = make_capture(from, bitboard::pop_LS1B(capture));
                move_counter++;
            }
        }
    }

    void generate_sliding_piece_moves(const BishopMap& atk_map, Move* move_list, int& move_counter, Bitboard piece_positions, Bitboard empty_squares, Bitboard opponent_pieces) {
        while (piece_positions) {
            Square from = pop_first(piece_positions);
            Bitboard atk_bb = atk_map.attacks(from, ~empty_squares);

            Bitboard quiet = atk_bb & empty_squares;
            while (quiet)
            {
                *move_list++ = make_quiet(from, bitboard::pop_LS1B(quiet));
                move_counter++;
            }

            Bitboard capture = atk_bb & opponent_pieces;
            while (capture)
            {
                *move_list++ = make_capture(from, bitboard::pop_LS1B(capture));
                move_counter++;
            }
        }
    }

    void generate_sliding_piece_moves(const QueenMap& atk_map, Move* move_list, int& move_counter, Bitboard piece_positions, Bitboard empty_squares, Bitboard opponent_pieces) {
        while (piece_positions) {
            Square from = pop_first(piece_positions);
            Bitboard atk_bb = atk_map.attacks(from, ~empty_squares);

            Bitboard quiet = atk_bb & empty_squares;
            while (quiet)
            {
                *move_list++ = make_quiet(from, bitboard::pop_LS1B(quiet));
                move_counter++;
            }

            Bitboard capture = atk_bb & opponent_pieces;
            while (capture)
            {
                *move_list++ = make_capture(from, bitboard::pop_LS1B(capture));
                move_counter++;
            }
        }
    }


    int pop_1st_bit(Bitboard* bb) {

        Bitboard b = *bb ^ (*bb - 1);

        unsigned int fold = (unsigned)((b & 0xffffffff) ^ (b >> 32));
        *bb &= (*bb - 1);
        unsigned int fold2 = fold * 0x783a9b23;
        unsigned int bit_table_idx = fold2 >> 26;
        //return BitTable[(fold * 0x783a9b23) >> 26];
        return (fold * 0x783a9b23) >> 26;
    }

}

namespace pieces {

    PieceColour other_colour(PieceColour colour) {
        if (colour == PieceColour::WHITE) {
            return PieceColour::BLACK;
        }
        else if (colour == PieceColour::BLACK) {
            return PieceColour::WHITE;
        }
        else {
            return PieceColour::NULL_COLOUR;
        }
    }

    PieceType fen_char_to_piece_type(const char fen_char) {
        switch (fen_char) {
        case 'p':
        case 'P':
            return PieceType::PAWN;
        case 'n':
        case 'N':
            return PieceType::KNIGHT;
        case 'b':
        case 'B':
            return PieceType::BISHOP;
        case 'r':
        case 'R':
            return PieceType::ROOK;
        case 'q':
        case 'Q':
            return PieceType::QUEEN;
        case 'k':
        case 'K':
            return PieceType::KING;
        default:
            return PieceType::NULL_PIECE;
        }
    }

    PieceColour fen_char_to_piece_colour(const char fen_char) {
        switch (fen_char) {
        case 'p':
        case 'n':
        case 'b':
        case 'r':
        case 'q':
        case 'k':
            return PieceColour::BLACK;
        case 'P':
        case 'N':
        case 'B':
        case 'R':
        case 'Q':
        case 'K':
            return PieceColour::WHITE;
        default:
            return PieceColour::NULL_COLOUR;
        }
    }

    char piece_to_fen_char(const PieceType type, const PieceColour colour) {
        if (colour == PieceColour::WHITE) {
            return white_piece_type_to_fen_char(type);
        }
        else {
            return black_piece_type_to_fen_char(type);
        }
    }

    char black_piece_type_to_fen_char(const PieceType type) {
        switch (type) {
            case PieceType::PAWN:
                return 'p';
            case PieceType::KNIGHT:
                return 'n';
            case PieceType::BISHOP:
                return 'b';
            case PieceType::ROOK:
                return 'r';
            case PieceType::QUEEN:
                return 'q';
            case PieceType::KING:
                return 'k';
        }
    }

    char white_piece_type_to_fen_char(const PieceType type) {
        switch (type) {
            case PieceType::PAWN:
                return 'P';
            case PieceType::KNIGHT:
                return 'N';
            case PieceType::BISHOP:
                return 'B';
            case PieceType::ROOK:
                return 'R';
            case PieceType::QUEEN:
                return 'Q';
            case PieceType::KING:
                return 'K';
        }
    }



std::vector<std::pair<Square, Square>> Pawns::quiet_moves(Bitboard pawn_positions, Bitboard empty_squares) {
    std::vector<std::pair<Square, Square>> moves;
    if (pawn_positions == 0) {
        return moves;
    }
    std::vector<std::pair<Square, Square>> single_pushes = single_push(pawn_positions, empty_squares);
    std::vector<std::pair<Square, Square>> double_pushes = double_push(pawn_positions, empty_squares);
    std::copy(single_pushes.begin(), single_pushes.end(), std::back_inserter(double_pushes));
    return double_pushes;
}

std::vector<std::pair<Square, Square>> Pawns::captures(Bitboard pawn_positions, Bitboard opponent_pieces) {
    if (pawn_positions == 0) {
        return std::vector<std::pair<Square, Square>>();
    }
    std::vector<std::pair<Square, Square>> west_caps = west_captures(pawn_positions, opponent_pieces);
    std::vector<std::pair<Square, Square>> east_caps = east_captures(pawn_positions, opponent_pieces);
    std::copy(west_caps.begin(), west_caps.end(), std::back_inserter(east_caps));
    return east_caps;
}

std::vector<std::pair<Square, Square>> Pawns::single_push(Bitboard pawn_positions, Bitboard empty_squares) {
    Bitboard sources = push_sources(empty_squares) & pawn_positions;
    Bitboard targets = push_targets(sources);
    return zip_moves(sources, targets);
};

std::vector<std::pair<Square, Square>> Pawns::double_push(Bitboard pawn_positions, Bitboard empty_squares) {
    Bitboard targets = double_push_target() & empty_squares;
    Bitboard clear_to_target = push_sources(targets) & empty_squares;
    Bitboard sources = push_sources(clear_to_target) & pawn_positions;
    targets = push_targets(push_targets(sources));
    return zip_moves(sources, targets);
};

std::vector<std::pair<Square, Square>> Pawns::west_captures(Bitboard pawn_positions, Bitboard opponent_pieces) {
    Bitboard sources = west_attack_sources(opponent_pieces) & pawn_positions;
    Bitboard targets = west_attack_targets(sources);
    return zip_moves(sources, targets);
};

std::vector<std::pair<Square, Square>> Pawns::east_captures(Bitboard pawn_positions, Bitboard opponent_pieces) {
    Bitboard sources = east_attack_sources(opponent_pieces) & pawn_positions;
    Bitboard targets = east_attack_targets(sources);
    return zip_moves(sources, targets);
};

std::vector<std::pair<Square, Square>> Pawns::zip_moves(const Bitboard& sources, const Bitboard& targets) {
    std::vector<Square> serialised_sources = bitboard::scan_forward(sources);
    std::vector<Square> serialised_targets = bitboard::scan_forward(targets);
    std::vector<std::pair<Square, Square>> zipped_moves;

    for (int i = 0; i < serialised_sources.size(); i++) {
        zipped_moves.push_back(std::make_pair(serialised_sources[i], serialised_targets[i]));
    }

    return zipped_moves;
}

Bitboard Pawns::all_attacked_squares(Bitboard pawn_positions) {
    return east_attack_targets(pawn_positions) | west_attack_targets(pawn_positions);
}

Bitboard WhitePawns::push_targets(Bitboard sources) {
    return bitboard::north_one(sources);
};

Bitboard WhitePawns::push_sources(Bitboard targets) {
    return bitboard::south_one(targets);
};

Bitboard WhitePawns::east_attack_targets(Bitboard sources) {
    return bitboard::north_east_one(sources);
};

Bitboard WhitePawns::east_attack_sources(Bitboard opponent_pieces) {
    return bitboard::south_west_one(opponent_pieces);
};

Bitboard WhitePawns::west_attack_targets(Bitboard sources) {
    return bitboard::north_west_one(sources);
};

Bitboard WhitePawns::west_attack_sources(Bitboard opponent_pieces) {
    return bitboard::south_east_one(opponent_pieces);
};

Bitboard BlackPawns::push_targets(Bitboard sources) {
    return bitboard::south_one(sources);
};

Bitboard BlackPawns::push_sources(Bitboard targets) {
    return bitboard::north_one(targets);
};

Bitboard BlackPawns::east_attack_targets(Bitboard sources) {
    return bitboard::south_east_one(sources);
};

Bitboard BlackPawns::east_attack_sources(Bitboard opponent_pieces) {
    return bitboard::north_west_one(opponent_pieces);
};

Bitboard BlackPawns::west_attack_targets(Bitboard sources) {
    return bitboard::south_west_one(sources);
};

Bitboard BlackPawns::west_attack_sources(Bitboard opponent_pieces) {
    return bitboard::north_east_one(opponent_pieces);
};

/*
std::vector<std::pair<Square, Square>> JumpingPiece::quiet_moves(Bitboard piece_positions, Bitboard empty_squares) {
    std::vector<std::pair<Square, Square>> moves;
    if (piece_positions == 0) {
        return moves;
    }
    std::vector<Square> serialised_pieces = bitboard::scan_forward(piece_positions);
    for (auto piece : serialised_pieces) {
        Bitboard targets = attack_map->get(piece) & empty_squares;
        std::vector<Square> target_squares = bitboard::scan_forward(targets);
        for (auto target : target_squares) {
            moves.push_back(std::make_pair(piece, target));
        }
    }
    return moves;
};

std::vector<std::pair<Square, Square>> JumpingPiece::captures(Bitboard piece_positions, Bitboard occupied_squares, Bitboard opponent_pieces) {

    return quiet_moves(piece_positions, opponent_pieces);

};

Bitboard JumpingPiece::attacks(Bitboard piece_positions, Bitboard occupied_squares, Bitboard opponent_pieces) {
    if (piece_positions == 0) {
        return Bitboard{ 0 };
    }
    std::vector<Square> serialised_pieces = bitboard::scan_forward(piece_positions);
    Bitboard targets{ 0 };
    for (auto piece : serialised_pieces) {
        Bitboard friendly = occupied_squares & ~opponent_pieces;
        Bitboard squares_targeted = attack_map->get(piece);
        Bitboard squares_attacked = squares_targeted & ~friendly;
        targets |= squares_attacked;

    }
    return targets;
};

std::vector<std::pair<Square, Square>> SlidingPiece::quiet_moves(Bitboard piece_positions, Bitboard empty_squares) {
    std::vector<std::pair<Square, Square>> moves;
    if (piece_positions == 0) {
        return moves;
    }
    std::vector<Square> serialised_pieces = bitboard::scan_forward(piece_positions);
    for (auto piece : serialised_pieces) {
        Bitboard targets{ 0 };
        for (auto ray : rays) {
            targets |= ray->get(piece, ~empty_squares) & empty_squares;
        }
        std::vector<Square> target_squares = bitboard::scan_forward(targets);
        for (auto target : target_squares) {
            moves.push_back(std::make_pair(piece, target));
        }
    }
    return moves;
};

std::vector<std::pair<Square, Square>> SlidingPiece::captures(Bitboard piece_positions, Bitboard occupied_squares, Bitboard opponent_pieces) {
    std::vector<std::pair<Square, Square>> captures;
    if (piece_positions == 0) {
        return captures;
    }
    std::vector<Square> serialised_pieces = bitboard::scan_forward(piece_positions);
    for (auto piece : serialised_pieces) {
        Bitboard targets{ 0 };
        for (auto ray : rays) {
            targets |= ray->get(piece, occupied_squares);
        }
        targets &= opponent_pieces;
        std::vector<Square> target_squares = bitboard::scan_forward(targets);
        for (auto target : target_squares) {
            captures.push_back(std::make_pair(piece, target));
        }
    }

    return captures;

};

Bitboard SlidingPiece::attacks(Bitboard piece_positions, Bitboard occupied_squares, Bitboard opponent_pieces) {
    if (piece_positions == 0) {
        return Bitboard{ 0 };
    }
    std::vector<Square> serialised_pieces = bitboard::scan_forward(piece_positions);
    Bitboard targets{ 0 };
    for (auto piece : serialised_pieces) {
        for (auto ray : rays) {
            Bitboard friendly = occupied_squares & ~opponent_pieces;
            Bitboard squares_targeted = ray->get(piece, occupied_squares);
            Bitboard squares_attacked = squares_targeted & ~friendly;
            // This will return a ray upto and including the blocking piece
            targets |= squares_attacked;

            // Whether or not the blocking piece is included as a target that can 
            // be moved to (i.e., attacked) will depend on if the piece is friendly 
            // or not. If it is friendly, it is not attack, but otherwise it is.

            // friendly = occupied_squares & ~opponent_pieces
            // therfore, ~friendly = ~occupied_squares | opponent_pieces
            //targets &= (~occupied_squares | opponent_pieces);
        }
    }
    return targets;
}
*/
/*
std::unique_ptr<Pawns> StandardPieceFactory::make_pawns(PieceColour colour) { 
    if (colour == PieceColour::BLACK) {
        return std::make_unique<BlackPawns>();
    }
    else {
        return std::make_unique<WhitePawns>();
    }
     
};

std::unique_ptr<Piece> StandardPieceFactory::make_piece(PieceType type) {
    switch (type) {
    case PieceType::KING:
        return std::make_unique<JumpingPiece>(&king_attacks);
    case PieceType::KNIGHT:
        return std::make_unique<JumpingPiece>(&knight_attacks);
    case PieceType::ROOK:
        return std::make_unique<SlidingPiece>(std::vector<attacks::Ray*>{ &north_ray, & east_ray, & south_ray, & west_ray });
    case PieceType::BISHOP:
        return std::make_unique<SlidingPiece>(std::vector<attacks::Ray*>{ &north_east_ray, & south_east_ray, & north_west_ray });
    case PieceType::QUEEN:
        return std::make_unique<SlidingPiece>(std::vector<attacks::Ray*>{ &north_ray, & east_ray, & south_ray, & west_ray, &north_east_ray, & south_east_ray, & north_west_ray });
    default:
        return std::make_unique<NullPiece>();
    }
    

};
*/

} // namespace pieces