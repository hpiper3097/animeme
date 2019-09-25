#ifndef DEFS_H
#define DEFS_H

#include "stdlib.h"
#include "stdio.h"

#define DEBUG

#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n) \
if(!(n)) { \
printf("%s - Failed", #n); \
printf("On %s", __DATE__); \
printf("At %s", __TIME__); \
printf("In file %s", __FILE__); \
printf("At line %d\n", __LINE__); \
exit(1); }
#endif

typedef unsigned long long U64;

#define NAME "Animeme 2.0"
#define BRD_SQ_NUM 120

#define MAXGAMEMOVES 2048
#define MAXPOSITIONMOVES 256
#define MAXDEPTH 64

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

//set piece values, and values for files and ranks
enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK  }; 
enum {FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE  };
enum {RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE  }; 

enum {WHITE, BLACK, BOTH}; //effective for labeling
enum { UCIMODE, XBOARDMODE, CONSOLEMODE };
enum {
	A1 = 21, B1, C1, D1, E1, F1, G1, H1,
	A2 = 31, B2, C2, D2, E2, F2, G2, H2,
	A3 = 41, B3, C3, D3, E3, F3, G3, H3,
	A4 = 51, B4, C4, D4, E4, F4, G4, H4,
	A5 = 61, B5, C5, D5, E5, F5, G5, H5,
	A6 = 71, B6, C6, D6, E6, F6, G6, H6,
	A7 = 81, B7, C7, D7, E7, F7, G7, H7,
	A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ, OFFBOARD
}; //board positions with sq120 indecies

enum { FALSE, TRUE  }; //implied

enum { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8 }; //castling enums

/*GAME MOVE*/

//how one integer stores information about a move 

/*

0000 0000 0000 0000 0000 0000 0111 1111 -> FROM is size 0x7F 
0000 0000 0000 0000 0011 1111 1000 0000 -> To // gotten via >> 7 0x7F
0000 0000 0000 0011 1100 0000 0000 0000 -> store captured piece (needs 12 values or 4 bits) >> 14 0xF
0000 0000 0000 0100 0000 0000 0000 0000 -> EnPas 0x40000
0000 0000 0000 1000 0000 0000 0000 0000 -> PawnStart 0x80000
0000 0000 1111 0000 0000 0000 0000 0000 -> was a Promoted Piece >> 20 0xF
0000 0001 0000 0000 0000 0000 0000 0000 -> was it a castle 0x1000000

*/

typedef struct {	
	int move;
	int score;	
} S_MOVE; //structure for movement
// 98 squares can be represented with 7 bits :: 1 2 4 8 16 32 64  1100010

typedef struct {
	S_MOVE moves[MAXPOSITIONMOVES];
	int count;
} S_MOVELIST;

typedef struct {
	
	int move;
	int castlePerm;
	int enPas;
	int fiftyMove;
	U64 hashKey;
	
} S_UNDO; //structure for undoing moves

typedef struct {
	U64 posKey;
	int move;
} S_PVENTRY;

typedef struct {
	S_PVENTRY* pTable;
	int numEntries;
} S_PVTABLE;


typedef struct {
	
	int pieces[BRD_SQ_NUM]; 
	U64 pawns[3];
	
	int KingSq[2];
	
	int side;
	int enPas;
	int fiftyMove;
	
	int ply;
	int hisPly;
	
	int castlePerm;
	
	U64 posKey;
	
	int pceNum[13];
	int bigPce[2];	//anything that is not a pawns
	int majPce[2];	//rooks and queens
	int minPce[2];	//bishops and knights
	int material[2]; // material scores

	S_UNDO history[MAXGAMEMOVES];
	
	//piecelsit
	int pList[13][10];
	
	// pList[wN][0] = E1 ---adds white knight to E1;
	// pList[wN][1] = D4 ---sets second white knight to D4;
	
	S_PVTABLE PvTable[1];
	int PvArray[MAXDEPTH];
	
	int searchHistory[13][BRD_SQ_NUM]; // each time move improves on alpha, for the piece type and to sq increment array
	int searchKillers[2][MAXDEPTH]; // stores most recent 2 moves that caused beta cut off that is not a capture
	
} S_BOARD; //structure for board positions

typedef struct {
	
	int starttime;
	int stoptime;
	int depth;
	int depthset;
	int timeset;
	int movestogo;
	int infinite;
	
	long nodes;
	
	int quit;
	int stopped;
	
	float fh;
	float fhf;
	
	int GAME_MODE;
	int POST_THINKING;
	
} S_SEARCHINFO;


#define FROMSQ(m) ((m) & 0x7F)
#define TOSQ(m) (((m)>>7) & 0x7F)
#define CAPTURED(m) (((m) >> 14) & 0xF)
#define PROMOTED(m) (((m) >> 20) & 0xF)

#define MFLAGEP 0x40000
#define MFLAGPS 0x80000
#define MFLAGCA 0x1000000

#define MFLAGCAP 0x7C000
#define MFLAGPROM 0xF00000

//macros
#define FR2SQ(f,r) ( (21 + (f) ) + ( (r) * 10 ) ) // gives square on board for 120 grid given file and rank
#define SQ64(sq120) (Sq120ToSq64[sq120]) //macro to abreviate name sq120tosq64
#define SQ120(sq64) (Sq64ToSq120[sq64]) // same as that but not^^
#define POP(b) PopBit(b) //macro to abreviate
#define CNT(b) CountBits(b) // macro to abreviate
#define CLRBIT(bb, sq) ((bb) &= ClearMask[(sq)]) //macro to abreviate
#define SETBIT(bb, sq) ((bb) |= SetMask[(sq)]) // macro to abreviate

#define IsBQ(p) (PieceBishopQueen[p])
#define IsRQ(p) (PieceRookQueen[p])
#define IsKn(p) (PieceKnight[p])
#define IsKi(p) (PieceKing[p])

#define NOMOVE 0

#define MIRROR64(sq) (Mirror64[sq])

// globals
extern int Sq120ToSq64[BRD_SQ_NUM];
extern int Sq64ToSq120[64];
extern U64 SetMask[64];
extern U64 ClearMask[64];
extern U64 PieceKeys[13][120];
extern U64 SideKey;
extern U64 CastleKeys[16];

extern int FilesBrd[BRD_SQ_NUM]; //yields file/rank for any 120 based square number; holds OFFBOARD at indecies outside the board scope
extern int RanksBrd[BRD_SQ_NUM];

/*  begin data.c globals */
extern char PceChar[]; // each is a string holding values for parsing
extern char SideChar[];
extern char RankChar[];
extern char FileChar[];
// all are booleans at indecies representing pieces, except PieceVal, which holds piece values at each piece index with i=0 == FALSE
extern int PieceBig[13];
extern int PieceMaj[13];
extern int PieceMin[13];
extern int PieceVal[13];
extern int PieceCol[13];
extern int PiecePawn[13];

extern int PieceKnight[13];
extern int PieceKing[13];
extern int PieceRookQueen[13];
extern int PieceBishopQueen[13];
extern int PieceSlides[13];

extern int Mirror64[64];

extern U64 FileBBMask[8];
extern U64 RankBBMask[8];

extern U64 BlackPassedMask[64];
extern U64 WhitePassedMark[64];
extern U64 IsolatedMask[64];
/* end */


// FUNCTIONS //

//init.c
extern void AllInit();

//bitboards.c
extern void PrintBitBoard(U64 bb);
extern int PopBit(U64 *bb);
extern int CountBits(U64 b);

//hashkey.c
extern U64 GeneratePosKey(const S_BOARD* pos);

//board.c
extern void ResetBoard(S_BOARD *pos);
extern int ParseFen(char *fen, S_BOARD *pos);
extern void PrintBoard(const S_BOARD* pos);
extern void UpdateListMaterial(S_BOARD* pos);
extern int CheckBoard(const S_BOARD* pos);
extern void MirrorBoard(S_BOARD* pos);

//attack.c 
extern int SqAttacked(const int sq, const int side, const S_BOARD* pos);

//io.c
extern char* PrSq(const int sq);
extern char* PrMove(const int move);
extern void PrintMoveList(const S_MOVELIST*list);
extern int ParseMove(char *ptrChar, S_BOARD* pos);

//movegen.c 
extern void GenerateAllMoves(const S_BOARD* pos, S_MOVELIST* list);
extern void GenerateAllCaps(const S_BOARD* pos, S_MOVELIST* list);
extern int MoveExists(S_BOARD* pos, const int move);
extern int InitMvvLva();

//validate.c 
extern int SqOnBoard(const int sq);
extern int SideValid(const int side);
extern int FileRankValid(const int fr);
extern int PieceValidEmpty(const int pce);
extern int PieceValid(const int pce);

//makemove.c 
extern int MakeMove(S_BOARD *pos, int move);
extern void TakeMove(S_BOARD* pos);

//perft.c 
extern void PerftTest(int depth, S_BOARD* pos);

//search.c 
extern void SearchPositions(S_BOARD* pos, S_SEARCHINFO* info);

//misc.c 
extern int GetTimeMs();
extern void ReadInput(S_SEARCHINFO *info);

//pvtable.c 
extern void InitPvTable(S_PVTABLE* table);
extern void StorePvMove(const S_BOARD* pos, const int move);
extern int ProbePvTable(const S_BOARD* pos);
extern int GetPvLine(const int depth, S_BOARD* pos);
extern void ClearPvTable(S_PVTABLE* table);

//evaluate.c
extern int EvalPosition(const S_BOARD* pos);

//uci.c 
extern void UCI_Loop(S_BOARD* pos, S_SEARCHINFO* info);

// xboard.c 
extern void Xboard_Loop(S_BOARD *pos, S_SEARCHINFO *info);
extern void Console_Loop(S_BOARD *pos, S_SEARCHINFO *info);

#endif


