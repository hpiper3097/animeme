// init.c 

#include "defs.h"
#include "stdlib.h"
#include "stdio.h"

//rand generates a 15 bit random number, so it pushes rand() 4 times for 60 bits
//the last line AND's with 0xf which is a 32 bit binary that is all 0's except the last 4 digits which are 1's
#define RAND_64 (	(U64)rand() | \
					(U64)rand() << 15 | \
					(U64)rand() << 30 | \
					(U64)rand() << 45 | \
					((U64)rand() & 0xf) << 60	)

int Sq120ToSq64[BRD_SQ_NUM];
int Sq64ToSq120[64];

U64 SetMask[64];
U64 ClearMask[64];

U64 PieceKeys[13][120];
U64 SideKey;
U64 CastleKeys[16]; //16 is sums of castling enum values in defs.h

int FilesBrd[BRD_SQ_NUM];
int RanksBrd[BRD_SQ_NUM];

U64 FileBBMask[8];
U64 RankBBMask[8];

U64 BlackPassedMask[64];
U64 WhitePassedMark[64];
U64 IsolatedMask[64];

void InitEvalMasks() {
	
	int sq, tsq, r, f;
	
	for(sq = 0; sq < 8; ++sq) {		
        FileBBMask[sq] = 0ULL; 
		RankBBMask[sq] = 0ULL; 
	}
	
	for(r = RANK_8; r >= RANK_1; r--) {
        for (f = FILE_A; f <= FILE_H; f++) {
            sq = r * 8 + f;
            FileBBMask[f] |= (1ULL << sq);
            RankBBMask[r] |= (1ULL << sq);
        }
	}
	
	for(sq = 0; sq < 64; ++sq) {
		IsolatedMask[sq] = 0ULL; 
		WhitePassedMark[sq] = 0ULL; 
		BlackPassedMask[sq] = 0ULL;
    }

	for(sq = 0; sq < 64; ++sq) {
		tsq = sq + 8;
		
        while(tsq < 64) {
            WhitePassedMark[sq] |= (1ULL << tsq);
            tsq += 8;
        }

        tsq = sq - 8;
        while(tsq >= 0) {
            BlackPassedMask[sq] |= (1ULL << tsq);
            tsq -= 8;
        }

        if(FilesBrd[SQ120(sq)] > FILE_A) {
            IsolatedMask[sq] |= FileBBMask[FilesBrd[SQ120(sq)] - 1];

            tsq = sq + 7;
            while(tsq < 64) {
                WhitePassedMark[sq] |= (1ULL << tsq);
                tsq += 8;
            }

            tsq = sq - 9;
            while(tsq >= 0) {
                BlackPassedMask[sq] |= (1ULL << tsq);
                tsq -= 8;
            }
        }
		
        if(FilesBrd[SQ120(sq)] < FILE_H) {
            IsolatedMask[sq] |= FileBBMask[FilesBrd[SQ120(sq)] + 1];

            tsq = sq + 9;
            while(tsq < 64) {
                WhitePassedMark[sq] |= (1ULL << tsq);
                tsq += 8;
            }

            tsq = sq - 7;
            while(tsq >= 0) {
                BlackPassedMask[sq] |= (1ULL << tsq);
                tsq -= 8;
            }
        }
	}	
}

void InitFilesRanksBrd() {
	
	int index = 0;
	int file = FILE_A;
	int rank = RANK_1;
	int sq = A1;
	int sq64 = 0;
	
	for(index=0; index < BRD_SQ_NUM; ++index) {
		FilesBrd[index] = OFFBOARD;
		RanksBrd[index] = OFFBOARD;
	}
	
	for(rank=RANK_1;rank<=RANK_8;++rank) {
		for(file=FILE_A;file<=FILE_H;++file) {
			sq = FR2SQ(file, rank);
			FilesBrd[sq] = file;
			RanksBrd[sq] = rank;
		}
	}
	
}

void InitHashKeys() {
	int index = 0;
	int index2 = 0;
	for(index = 0; index < 13; ++index) {
		for(index2 = 0; index2 < 120; ++index2) {
			PieceKeys[index][index2] = RAND_64;
		}
	}
	SideKey = RAND_64;
	for(index=0;index<16;++index){
		CastleKeys[index] = RAND_64;
	}
}

void InitBitMasks() {
	int index = 0;
	
	for(index=0; index<64;index++) {
		SetMask[index] = 0ULL;
		ClearMask[index] = 0ULL;
	}
	
	for(index=0;index<64;index++) {
		SetMask[index] = (1ULL << index); //pushes a one to the index # 
		ClearMask[index] = ~SetMask[index]; // clearmask = complement (~) of setmask
	}
	
}

void InitSq120to64() {
	
	int index = 0;
	int file = FILE_A;
	int rank = RANK_1;
	int sq = A1;
	int sq64 = 0;
	for(index = 0; index < BRD_SQ_NUM; ++index) {
		Sq120ToSq64[index] = 65;
	}
	
	for(index = 0; index < 64; ++index) {
		Sq64ToSq120[index] = 120;
	}
	
	for(rank = RANK_1; rank <= RANK_8; ++rank) {
		for(file = FILE_A; file <= FILE_H; ++file) {
			sq = FR2SQ(file, rank);
			Sq64ToSq120[sq64] = sq;
			Sq120ToSq64[sq] = sq64;
			sq64++;
		}
	}
}

void AllInit() {
	//what the fuck
	InitSq120to64();
	InitBitMasks();
	InitHashKeys();
	InitFilesRanksBrd();
	InitEvalMasks();
	InitMvvLva();
}