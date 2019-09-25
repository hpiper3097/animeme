//perft.c 

#include "defs.h"
#include "stdio.h"

long leafNodes;

void Perft(int depth, S_BOARD* pos) {
	
	
	
	if(depth == 0) {
		leafNodes++;
		return;
	}
	
	S_MOVELIST list[1];
	GenerateAllMoves(pos, list);
	
	int MoveNum = 0;
	for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
		if(!MakeMove(pos, list->moves[MoveNum].move) ){
			continue;
		}
		/* FIX AFTER DONE WITH CODE I DON'T THINK I NEED RECURSION*/
		Perft(depth -1, pos); 
		TakeMove(pos);
	}
	
	ASSERT(CheckBoard(pos));
	
	return;
}

void PerftTest(int depth, S_BOARD* pos) {
	
	ASSERT(CheckBoard(pos));
	
	PrintBoard(pos);
	printf("\nStarting Test To Depth:%d\n",depth);
	leafNodes = 0;
	int start = GetTimeMs();
	
	S_MOVELIST list[1];
	GenerateAllMoves(pos, list);
	
	int move;
	int MoveNum = 0;
	for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
		move = list->moves[MoveNum].move;
		if(!MakeMove(pos, move)) {
			continue;
		}
		long cumnodes = leafNodes;
		Perft(depth - 1, pos);
		TakeMove(pos);
		long oldnodes = leafNodes - cumnodes;
		if(MoveNum+1 <10)
			printf("move %d  : %s : %ld\n", MoveNum+1, PrMove(move), oldnodes);
		else
			printf("move %d : %s : %ld\n", MoveNum+1, PrMove(move), oldnodes);
	}
	
	printf("\nTestComplete : %ld nodes visited in %dms \n", leafNodes, GetTimeMs() - start);
	
	return;
	
}