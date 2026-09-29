#pragma once
#include <cstdint>
#include <cstdlib>
#include <cmath>

typedef std::uint64_t Bitboard;

enum class Piece {
    ROOK, KING, QUEEN, BISHOP, PAWN, KNIGHT
};

enum class Square : uint8_t {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8
};

constexpr bool is_valid_square(const Square& square) {
    return static_cast<uint8_t>(square) < 64;
}

constexpr Bitboard square_to_BB(const Square& square) {
    return 1ULL << static_cast<uint8_t>(square);
}

constexpr inline bool is_square_occupied(const Square& square, const Bitboard& occ) {
    return (square_to_BB(square) & occ) != 0;
}

constexpr inline int get_file(Square square) {
    return static_cast<uint8_t>(square) % 8;
}

constexpr inline int get_rank(Square square) {
    return static_cast<uint8_t>(square) / 8;
}

constexpr inline int abs_int(int x) {
    return x < 0 ? -x : x;
}

constexpr inline bool valid_distance_check(Square orginal, Square newSquare, Piece piece) {
    const int fileDistance = abs_int(get_file(orginal) - get_file(newSquare));
    const int rankDistance = abs_int(get_rank(orginal) - get_rank(newSquare));


    switch (piece)
    {
    case Piece::KING:
        
        return (
            fileDistance <= 1 &&
            rankDistance <= 1 &&
            (rankDistance !=0 || fileDistance != 0)
        );

    case Piece::KNIGHT:
        return (
            (fileDistance == 1 && rankDistance == 2) ||
            (fileDistance == 2 && rankDistance == 1)
        );

    case Piece::ROOK:
        return (
            (fileDistance == 0 || rankDistance == 0) && (fileDistance != 0 || rankDistance != 0)
        );

    case Piece::BISHOP:
        return (fileDistance != 0 || rankDistance != 0) && fileDistance == rankDistance;

    case Piece::QUEEN:
        return (fileDistance != 0 || rankDistance != 0) && (fileDistance == 0 || rankDistance == 0 || fileDistance == rankDistance);

    default:
        return false;
    }
}

enum class Colour {
    White,
    Black
};

namespace StartingPosition {

constexpr Bitboard WHITE_PAWNS   = 0x000000000000FF00;
constexpr Bitboard WHITE_ROOKS   = 0x0000000000000081;
constexpr Bitboard WHITE_KNIGHTS = 0x0000000000000042;
constexpr Bitboard WHITE_BISHOPS = 0x0000000000000024;
constexpr Bitboard WHITE_QUEEN   = 0x0000000000000008;
constexpr Bitboard WHITE_KING    = 0x0000000000000010;

constexpr Bitboard BLACK_PAWNS   = 0x00FF000000000000;
constexpr Bitboard BLACK_ROOKS   = 0x8100000000000000;
constexpr Bitboard BLACK_KNIGHTS = 0x4200000000000000;
constexpr Bitboard BLACK_BISHOPS = 0x2400000000000000;
constexpr Bitboard BLACK_QUEEN   = 0x0800000000000000;
constexpr Bitboard BLACK_KING    = 0x1000000000000000;

}