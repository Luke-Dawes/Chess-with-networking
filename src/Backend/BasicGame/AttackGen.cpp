#include "AttackGen.h"



Bitboard AttackGen::kingAttacks(Square square) {
    constexpr int possible_Moves[8] = {+1, -1, +7, +8 ,+9, -7, -8, -9};

    Bitboard potentialMoves = 0ULL;

    for (const auto& move : possible_Moves) {

        Square newSqure = static_cast<Square>(static_cast<uint8_t>(square) + static_cast<uint8_t>(move));

        if (!is_valid_square(newSqure) || !valid_distance_check(square, newSqure, Piece::KING)) continue;

        potentialMoves |= square_to_BB(newSqure);
    }
    return potentialMoves;
}

Bitboard AttackGen::rookAttacks(Square square, Bitboard& occupied) {
    constexpr int possibleMoves[4] = {+1, -1, +8, -8};

    Bitboard potentialMoves = 0ULL;
    Square NewSquare;

    for (const auto& move : possibleMoves) {

        NewSquare = static_cast<Square>(static_cast<uint8_t>(square) + move);

        while (is_valid_square(NewSquare)) {

            if (is_square_occupied(NewSquare, occupied) || !valid_distance_check(square, NewSquare, Piece::ROOK)) break;

            potentialMoves |= square_to_BB(NewSquare);
            
            NewSquare = static_cast<Square>(static_cast<uint8_t>(NewSquare) + move);
        }

    }
    return potentialMoves;
}

Bitboard AttackGen::knightAttacks(Square square) {
    constexpr int possibleMoves[8] = {+17, +15, +10, +6, -6,  -10, -15, -17};

    Bitboard potentialMoves = 0ULL;
    
    for (const auto& move : possibleMoves) {

        Square newSquare = static_cast<Square>(static_cast<uint8_t>(square) + move);

        if (!is_valid_square(newSquare) || !valid_distance_check(square, newSquare, Piece::KNIGHT)) continue;

        potentialMoves |= square_to_BB(newSquare);
    }

    return potentialMoves;
}