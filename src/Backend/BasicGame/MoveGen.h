#pragma once

#include <cstdint>
#include "LookupTables.h"
#include "Helper.h"
#include "MoveGen.h"
#include "AttackGen.h"
#include <stdexcept>

enum PieceType {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, NO_PIECE};

using Move = uint16_t;

/*
    Move: 
        bits 0-5 is the from square (0-63)
        bits 6-11 is the to square (0-63)
        bits 12-15 are flags.

        Flags:
            4 bits.
            if bit 2 is set the move will capture something 
            if bit 3 is set the move is a promotion

            for promotions the lower 2 bits set the new piece 
                0=n
                1=bishop
                2=rook
                3=queen
*/


enum MoveFlag {
    QUIET = 0,  //normal move
    DOUBLE_PUSH = 1, //pawn moves twice
    KING_CASTLE = 2, //castle king side
    QUEEN_CASTLE = 3, //castle queen side
    CAPTURE = 4, //bit 2 capture
    EN_PASSANT = 5, //en-passant

    //promotion to bishop, rook, queen
    PROMO_N = 8, PROMO_B = 9, PROMO_R = 10, PROMO_Q = 11,  
    
    //promotion to bishop, rook, queen whilst capturing something
    PROMO_CAP_N = 12, PROMO_CAP_B = 13, PROMO_CAP_R = 14, PROMO_CAP_Q = 15
};


//create a move which can be sent to java
inline Move makeMoveInt(int from, int to, int flags) {
    return Move(from | (to << 6) | (flags << 12));
}
inline int moveFrom(Move m) {
    return m & 63;
}

inline int moveTo(Move m) {
    return (m >> 6) & 63;
}

inline int moveFlags(Move m) {
    return m >> 12;
}


//no board has more moves than 218 legal moves.
struct MoveList {
    Move moves[256];
    int count = 0; 
    void add(int from, int to, int flags) {moves[count++] = makeMoveInt(from, to, flags);}
};


struct Position
{
    Bitboard pieces[2][6] = {}; //[colour][type]


    Bitboard occ[2] = {}; //every piece of that colour - refresh when used
    Bitboard all = 0; //every piece on the board - refreshed when used
    int side = WHITE;

    /*
        castling rights as bits 
            1 white king side
            2 white queen side
            4 black king side
            8 black queen side
    */
    int castling = 0;

    int ep = -1; //en passant target square or -1 for none

    //compute occ and all
    void refresh() {
        occ[0] = 0;
        occ[1] = 0;

        for (int p = 0; p < 6; ++p) { occ[0] |= pieces[0][p]; occ[1] |= pieces[1][p]; }
        all = occ[0] | occ[1];
    }

};

//finds the lowest set bit of b
inline int lsb(Bitboard b) {
    unsigned long idx;
    _BitScanForward64(&idx, b);
    return (int)idx;
}

//returns the lowest bits index from b and returns the index - used to loop over every piece
inline int popLsb(Bitboard& b) { 
    int s = lsb(b);
     b &= b - 1; 
     return s; 
}

//starting positions
inline Position startPosition() {
    Position p;
    p.pieces[WHITE][PAWN]   = 0x000000000000FF00ULL;
    p.pieces[WHITE][KNIGHT] = 0x0000000000000042ULL;
    p.pieces[WHITE][BISHOP] = 0x0000000000000024ULL;
    p.pieces[WHITE][ROOK]   = 0x0000000000000081ULL;
    p.pieces[WHITE][QUEEN]  = 0x0000000000000008ULL;
    p.pieces[WHITE][KING]   = 0x0000000000000010ULL;
    p.pieces[BLACK][PAWN]   = 0x00FF000000000000ULL;
    p.pieces[BLACK][KNIGHT] = 0x4200000000000000ULL;
    p.pieces[BLACK][BISHOP] = 0x2400000000000000ULL;
    p.pieces[BLACK][ROOK]   = 0x8100000000000000ULL;
    p.pieces[BLACK][QUEEN]  = 0x0800000000000000ULL;
    p.pieces[BLACK][KING]   = 0x1000000000000000ULL;
    p.castling = 15;
    p.refresh();
    return p;
}

inline bool isAttacked(const Position& p, int sq, int by) {

    //is a square attacked by the other colour
    //computed backwards - i.e. if your rook is on the square, is it able to reach another rook.

    if (pawnTable[by ^ 1][sq] & p.pieces[by][PAWN]) return true;
    if (knightTable[sq] & p.pieces[by][KNIGHT]) return true;
    if (kingTable[sq] & p.pieces[by][KING]) return true;

    Bitboard diag = p.pieces[by][BISHOP] | p.pieces[by][QUEEN];
    Bitboard line = p.pieces[by][ROOK] | p.pieces[by][QUEEN];

    if (bishopAttacks(sq, p.all) & diag) return true;
    if (rookAttacks(sq, p.all) & line) return true;

    return false;
}


inline bool inCheck(const Position& p, int colour) {
    return isAttacked(p, lsb(p.pieces[colour][KING]), colour ^ 1);
}

//generate all pseduo legal moves (i.e. could leave us in check but are all possible moves)
inline void generatePseudoLegal(const Position& p, MoveList& out) {

    const int us = p.side;
    const int them = us ^ 1;

    const Bitboard own = p.occ[us];
    const Bitboard enemy = p.occ[them];
    const Bitboard empty = ~p.all;


    //pawns
    {

        const int push = (us == WHITE) ? 8 : -8;

        //squares that the pawn may land on if they double jump off the start
        const Bitboard doubleRank = (us == WHITE) ? 0x0000000000FF0000ULL : 0x0000FF0000000000UL;

        //range of sqaures that a pawn could be on to promote 
        const int promoMin = (us == WHITE) ? 56 : 0;
        const int promoMax = (us == WHITE) ? 63 : 7;

        Bitboard pawns = p.pieces[us][PAWN];

        Bitboard single = (us == WHITE ? pawns << 8 : pawns >> 8) & empty;
        Bitboard dbl = (us == WHITE ? (single & doubleRank) << 8 : (single & doubleRank) >> 8) & empty;

        Bitboard b = single;


        while (b) {
            int to = popLsb(b);
            int from = to - push;

            if (to >= promoMin && to <= promoMax) {

                //adds that it could be promoted 4 ways
                for (int fl = PROMO_N; fl <= PROMO_Q; ++fl) {
                    out.add(from, to, fl);
                }

            } 
            else {
                out.add(from, to, QUIET);
            }

        }

        b = dbl;

        while (b) {
            int to = popLsb(b);
            out.add(to - 2 * push, to, DOUBLE_PUSH);
        }


        b = pawns;
        while (b) {

            int from = popLsb(b);
            Bitboard caps = pawnTable[us][from] & enemy;

            while (caps) {
                int to = popLsb(caps);

                if (to >= promoMin && to <= promoMax) {
                    for (int fl = PROMO_CAP_N; fl <= PROMO_CAP_Q; ++fl) {
                        out.add(from, to, fl);
                    }
                } 
                else {
                    out.add(from, to, CAPTURE);
                }
            }

            if (p.ep != -1 && (pawnTable[us][from] & (1ULL << p.ep))) {
                out.add(from, p.ep, EN_PASSANT);
            }

        }

    }


    //knights, bishops, rooks, queens, king

    for (int pt = KNIGHT; pt <= KING; ++pt) {

        Bitboard b = p.pieces[us][pt];

        while (b) {

            int from = popLsb(b);
            Bitboard attack;

            switch(pt) {
                case KNIGHT: attack = knightTable[from]; break;
                case BISHOP: attack = bishopAttacks(from, p.all); break;
                case ROOK: attack = rookAttacks(from, p.all); break;
                case QUEEN: attack = bishopAttacks(from, p.all) | rookAttacks(from, p.all); break;
                case KING: attack = kingTable[from]; break;
                default: throw std::runtime_error("unknown piece");
            }

            attack &= ~own;

            Bitboard caps = attack & enemy;
            Bitboard quiet = attack & ~enemy;

            while (caps) {
                out.add(from, popLsb(caps), CAPTURE);
            }

            while (quiet) {
                out.add(from, popLsb(quiet), QUIET);
            }


        }

    }
        //castling 
        {
            const int base = (us == WHITE) ? 0 : 56;
            const int kBit = (us == WHITE) ? 1 : 4;
            const int qBit = (us == WHITE) ? 2 : 8;
            const int e = base + 4;

            if ((p.castling & (kBit | qBit)) && !isAttacked(p, e, them)) {

                if ((p.castling & kBit) && 
                !(p.all & ((1ULL << (base + 5) | 1ULL << (base + 6)))) &&
                !isAttacked(p, base + 5, them)) {

                    out.add(e, base + 6, KING_CASTLE);
                }

                if ((p.castling & qBit) &&
                !(p.all & ((1ULL << (base + 1)) | (1ULL << (base + 2)) | (1ULL << (base + 3)))) &&
                !isAttacked(p, base + 3, them)) {

                    out.add(e, base + 2, QUEEN_CASTLE);
                }


            }

        }
    }


    //make the moves
inline int pieceAt(const Position& p, int colour, int sq) {

    for (int pt = 0; pt < 6; ++pt) {

        if (p.pieces[colour][pt] & (1ULL << sq)) {
            return pt;
        }
    }
        return NO_PIECE;
}

inline void makeMove(Position& p, Move m ) {

    const int us = p.side;
    const int them = us ^ 1;

    const int from = moveFrom(m);
    const int to = moveTo(m);
    const int flags = moveFlags(m);

    const Bitboard fromBB = 1ULL << from;
    const Bitboard toBB = 1ULL << to;

    int pt = pieceAt(p, us, from);

    if (flags == EN_PASSANT) {

        p.pieces[them][PAWN] &= ~(1ULL << (us == WHITE ? to - 8 : to + 8));

    } else if (flags & CAPTURE) {
        int cp = pieceAt(p, them, to);
        p.pieces[them][cp] &= ~toBB;
    }

    p.pieces[us][pt] &= ~fromBB;
    if (flags & 8) {
        pt = KNIGHT + (flags & 3);
    }
    p.pieces[us][pt] |= toBB;


    if (flags == KING_CASTLE) {
        p.pieces[us][ROOK] ^= (1ULL << (to + 1)) | (1ULL << (to - 1));
    } else if (flags == QUEEN_CASTLE) {
        p.pieces[us][ROOK] ^= (1ULL << (to - 2)) | (1ULL << (to + 1));
    }
 
    //lose castling rights if the square is touched 
    auto strip = [&](int sq) {
        switch (sq) {
            case 4:  p.castling &= ~3;  break;
            case 7:  p.castling &= ~1;  break;
            case 0:  p.castling &= ~2;  break;
            case 60: p.castling &= ~12; break;
            case 63: p.castling &= ~4;  break;
            case 56: p.castling &= ~8;  break;
        }
    };
    strip(from); strip(to);
 
    p.ep = (flags == DOUBLE_PUSH) ? (from + to) / 2 : -1;
    p.side = them;
    p.refresh();
}
 
//legal moves, generate all possible moves, and then if the king is in check in the new move drop it
inline void generateLegal(const Position& p, MoveList& out) {
    MoveList pseudo;
    generatePseudoLegal(p, pseudo);

    for (int i = 0; i < pseudo.count; ++i) {
        Position next = p;
        makeMove(next, pseudo.moves[i]);

        if (!inCheck(next, p.side)) 
            out.moves[out.count++] = pseudo.moves[i];
    }
}
 
//return all moves from a single square - useful for clicking a square on the gui and it showing moves
inline void movesFrom(const Position& p, int sq, MoveList& out) {
    MoveList all;
    generateLegal(p, all);
    for (int i = 0; i < all.count; ++i)
        if (moveFrom(all.moves[i]) == sq) out.moves[out.count++] = all.moves[i];
}