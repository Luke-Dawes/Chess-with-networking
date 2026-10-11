#pragma once
#include <cstdint>

constexpr uint64_t nextRandom(uint64_t& state) {
    //SplitMix64 for compile time generation

    uint64_t z = (state += 0x9E3779B97F4A7C15ULL);
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}

struct zobhristTables  {
    //colour type square
    uint64_t pieces[2][6][64]{};

    uint64_t sideToMove{};

    uint64_t castling[16]{};

    uint64_t enPassent[8]{};
};


constexpr zobhristTables generate() {
    zobhristTables table{};

     uint64_t state = 0x1234ABCDULL;

    for (int colour = 0; colour < 2; colour++) {

        for (int piece = 0; piece < 6; piece++) {

            for (int square = 0; square < 64; square++) {

                table.pieces[colour][piece][square] = nextRandom(state);

            }
        }
    }

    

    table.sideToMove = nextRandom(state);

    for (int i = 0; i < 16; i++) {
        table.castling[i] = nextRandom(state);
    }

    for (int i = 0; i < 8; i++) {
        table.enPassent[i] = nextRandom(state);
    }

    return table;
}



inline constexpr zobhristTables zobrist = generate();