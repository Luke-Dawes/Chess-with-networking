#include "AttackGen.h"
#include "LookupTables.h"
#include "Helper.h"

// Bitboard pawnAttacks(Square square, Colour colour) {
//     return pawnTable[colour][(uint8_t)square];
// }

// Bitboard kingAttacks(Square square) {
//     return kingTable[(uint8_t)square];
// }

// Bitboard rookAttacks(Square square,  Bitboard occupied) {
//     return rookTable[(uint8_t)square][_pext_u64(occupied, rookMask[(uint8_t)square])];
// }

// Bitboard bishopAttacks(Square square, Bitboard occupied) {
//     return bishopTable[(uint8_t)square][_pext_u64(occupied, bishopMask[(uint8_t)square])];
// }

// Bitboard queenAttacks(Square square, Bitboard occupied) {
//     return rookAttacks(square, occupied) | bishopAttacks(square, occupied);
// }

// Bitboard knightAttacks(Square square) {
//     return knightTable[(uint8_t)square];
// }

//int versions
Bitboard kingAttacks(int square) {
    return kingTable[(uint8_t)square];
}

Bitboard rookAttacks(int square,  Bitboard occupied) {
    return rookTable[(uint8_t)square][_pext_u64(occupied, rookMask[(uint8_t)square])];
}

Bitboard bishopAttacks(int square, Bitboard occupied) {
    return bishopTable[(uint8_t)square][_pext_u64(occupied, bishopMask[(uint8_t)square])];
}

Bitboard queenAttacks(int square, Bitboard occupied) {
    return rookAttacks(square, occupied) | bishopAttacks(square, occupied);
}

Bitboard knightAttacks(int square) {
    return knightTable[(uint8_t)square];
}

Bitboard pawnAttacks(int square, Colour colour) {
    return pawnTable[colour][(uint8_t)square];
}