#pragma once
#include "Helper.h"

    //these generate all possible moves, i.e. knight will always produce 8 possible moves if in the middle of the map

    //knight doesnt need occupied bc it always has max 8 moves.
    //rook needs it it will get blocked by the first piece, meaning it cant produce anymore moves

// Bitboard pawnAttacks(Square square, Colour colour);
// Bitboard knightAttacks(Square square);
// Bitboard bishopAttacks(Square square,  Bitboard occupied);
// Bitboard rookAttacks(Square square,  Bitboard occupied);
// Bitboard queenAttacks(Square square,  Bitboard occupied);
// Bitboard kingAttacks(Square square);

Bitboard pawnAttacks(int square, Colour colour);
Bitboard knightAttacks(int square);
Bitboard bishopAttacks(int square, Bitboard occupied);
Bitboard rookAttacks(int square, Bitboard occupied);
Bitboard queenAttacks(int square, Bitboard occupied);
Bitboard kingAttacks(int square);