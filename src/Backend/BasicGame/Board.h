#pragma once
#include <cstdint>
#include "Helper.h"

class Board {
    private:

    Bitboard whitePieces;
    Bitboard blackPieces;

    //where each black piece is
    Bitboard Blackpawns = StartingPosition::BLACK_PAWNS;
    Bitboard Blackrooks = StartingPosition::BLACK_ROOKS;
    Bitboard Blackknights = StartingPosition::BLACK_KNIGHTS;
    Bitboard Blackbishops = StartingPosition::BLACK_BISHOPS;
    Bitboard Blackqueen = StartingPosition::BLACK_QUEEN;
    Bitboard Blackking = StartingPosition::BLACK_KING;

    Bitboard Whitepawns = StartingPosition::WHITE_PAWNS;
    Bitboard Whiterooks = StartingPosition::WHITE_ROOKS;
    Bitboard Whiteknights = StartingPosition::WHITE_KNIGHTS;
    Bitboard Whitebishops = StartingPosition::WHITE_BISHOPS;
    Bitboard Whitequeen = StartingPosition::WHITE_QUEEN;
    Bitboard Whiteking = StartingPosition::WHITE_KING;
};
