#pragma once    
#include <cstdint>
#include <immintrin.h>

typedef uint64_t Bitboard;



extern Bitboard rookMask[64], bishopMask[64];
extern Bitboard rookTable[64][4096];
extern Bitboard bishopTable[64][512];
extern Bitboard knightTable[64];
extern Bitboard kingTable[64];
extern Bitboard pawnTable[2][64];

inline bool onBoard(int f, int r) {return f >= 0 && f < 8 && r >= 0 && r < 8;}

inline auto add = [](Bitboard& bb, int f, int r) { //takes bb as refrence so dont need to return
    if (onBoard(f, r)) bb |= 1ULL << (r * 8 + f);
};

struct Dir { int df, dr; };

constexpr Dir ROOK_DIRS[4] = {{1, 0}, {-1, 0}, {0, 1}, {0,-1}};
constexpr Dir BISHOP_DIRS[4] =  {{1,1}, {1,-1}, {-1,1}, {-1,-1}};

Bitboard slidingAttacks(int sq, Bitboard occ, const Dir (&dirs)[4]);

Bitboard relevanceMask(int sq, const Dir (&dirs)[4]);

void initSliders();
void initOther();

inline void initAttacks() {initSliders(); initOther();}