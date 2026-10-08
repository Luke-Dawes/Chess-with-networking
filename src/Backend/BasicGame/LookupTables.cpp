#include "LookupTables.h"
#include "Helper.h"

Bitboard rookMask[64], bishopMask[64];
Bitboard rookTable[64][4096];
Bitboard bishopTable[64][512];
Bitboard knightTable[64];
Bitboard kingTable[64];
Bitboard pawnTable[2][64];



Bitboard slidingAttacks(int sq, Bitboard occ, const Dir (&dirs)[4]) {
    Bitboard attacks = 0;

    for (const Dir& d : dirs) {

        int f = sq % 8 + d.df; 
        int r = sq / 8 + d.dr;

        while (onBoard(f, r)) {

            Bitboard bit = 1ULL << (r * 8 + f);
            attacks |= bit;

            if (occ & bit) break; //if square already occ end
            f += d.df;
            r += d.dr;

        }
    }
    return attacks;
}


Bitboard relevanceMask(int sq, const Dir (&dirs)[4]) {

    Bitboard mask = 0;

    for (const Dir& d : dirs) {
        int f = sq % 8 + d.df;
        int r = sq / 8 + d.dr;

        while (onBoard(f + d.df, r + d.dr)) { //drops the square at the edge of the board

            mask |= 1ULL << (r * 8 + f);
            f += d.df;
            r += d.dr;
        }
    }
    return mask;
}

void initSliders() {

    for (int sq = 0; sq < 64; ++sq) {

        rookMask[sq] = relevanceMask(sq, ROOK_DIRS);
        bishopMask[sq] = relevanceMask(sq, BISHOP_DIRS);


        Bitboard subset = 0;

        do {

            rookTable[sq][_pext_u64(subset, rookMask[sq])] = slidingAttacks(sq, subset, ROOK_DIRS);
            
            subset = (subset - rookMask[sq]) & rookMask[sq];

        } while (subset);

        subset = 0;

        do {

            bishopTable[sq][_pext_u64(subset, bishopMask[sq])] = slidingAttacks(sq, subset, BISHOP_DIRS);

            subset = (subset - bishopMask[sq]) & bishopMask[sq];

        } while (subset);

    }

}

void initOther() {

    constexpr int KNIGHT[8][2] = {{1,2},{2,1},{2,-1},{1,-2},{-1,-2},{-2,-1},{-2,1},{-1,2}};
    constexpr int KING[8][2]   = {{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};

    for (int sq = 0; sq < 64; ++sq) {
        int f = sq % 8;
        int r = sq / 8;

        knightTable[sq] = 0;
        for (auto& m: KNIGHT) add(knightTable[sq], f + m[0], r+m[1]);

        kingTable[sq] = 0;
        for (auto& m : KING) add(kingTable[sq], f + m[0], r + m[1]);

        pawnTable[WHITE][sq] = 0;
        add(pawnTable[WHITE][sq], f - 1, r + 1);
        add(pawnTable[WHITE][sq], f + 1, r + 1);

        pawnTable[BLACK][sq] = 0;
        add(pawnTable[BLACK][sq], f - 1, r - 1);
        add(pawnTable[BLACK][sq], f + 1, r - 1);
    }

}