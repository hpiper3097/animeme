//atack.c 

#include "defs.h"
#include "stdio.h"

const int KnDir[8] = {-8, -19, -21, -12, 8, 19, 21, 12};
const int RkDir[4] = {-1, -10, 1, 10};
const int BiDir[4] = {-9, -11, 9, 11};
const int KiDir[8] = {-1, -10, 1, 10, -9, -11, 11, 9};

int SqAttacked(const int sq, const int side, const S_BOARD* pos) { //determine if a sqare is being attacked
	
	int piece, index, t_sq, dir;
	
	ASSERT(SqOnBoard(sq));
	ASSERT(SideValid(side));
	ASSERT(CheckBoard(pos));
	
	//pawns
	if(side == WHITE) {
		if(pos->pieces[sq-11] == wP || pos->pieces[sq-9] == wP) {
			return TRUE;
		}
	} else {
		if(pos->pieces[sq+11] == bP || pos->pieces[sq+9] == bP) {
			return TRUE;
		}
	}
	
	//knights
	for(index = 0; index < 8; ++index) {
		piece = pos->pieces[sq + KnDir[index]];
		if(piece!=OFFBOARD && IsKn(piece) && PieceCol[piece]==side) {
			return TRUE;
		}
	}
	
	//rooks,queens
	for(index = 0; index<4; ++index) {
		dir = RkDir[index];
		t_sq = sq + dir;
		piece = pos->pieces[t_sq];
		while(piece != OFFBOARD) {
			if(piece != EMPTY) {
				if(IsRQ(piece) && PieceCol[piece] == side) {
					return TRUE;
				}
				break;
			}
			t_sq += dir;
			piece = pos->pieces[t_sq];
		}
	}
	
	//bishops, queens
	for(index = 0; index<4; ++index) {
		dir = BiDir[index];
		t_sq = sq + dir;
		piece = pos->pieces[t_sq];
		while(piece != OFFBOARD) {
			if(piece != EMPTY) {
				if(IsBQ(piece) && PieceCol[piece] == side) {
					return TRUE;
				}
				break;
			}
			t_sq += dir;
			piece = pos->pieces[t_sq];
		}
	}
	
	//kings
	for(index = 0; index < 8; ++index) {
		piece = pos->pieces[sq + KiDir[index]];
		if(IsKi(piece) && PieceCol[piece]==side) {
			return TRUE;
		}
	}
	
	return FALSE;
	
}