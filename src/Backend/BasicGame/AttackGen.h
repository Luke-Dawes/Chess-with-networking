#pragma once
#include "Helper.h"

class AttackGen {
    public:

    //these generate all possible moves, i.e. knight will always produce 8 possible moves if in the middle of the map

    //knight doesnt need occupied bc it always has max 8 moves.
    //rook needs it it will get blocked by the first piece, meaning it cant produce anymore moves

    static Bitboard pawnAttacks(Square square, Colour colour);
    static Bitboard knightAttacks(Square square);
    static Bitboard bishopAttacks(Square square, Bitboard& occupied);
    static Bitboard rookAttacks(Square square, Bitboard& occupied);
    static Bitboard queenAttacks(Square square, Bitboard& occupied);
    static Bitboard kingAttacks(Square square);
};