#include<stdio.h>
#include<stdlib.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


void customAssert(int expression, char* errorMessage){

        if(expression != 1)
        {
                printf("Assertion Failed: %s\n",errorMessage);
            	fflush(stdout);
        }
}


int checkAdventurer(int p, struct gameState *post) {

	struct gameState pre;
	memcpy(&pre,post,sizeof(struct gameState));
//
	int r = -1;
	r =  adventurerEffect(0, post, p,0); 

//	custom assert test values that should not change 
	customAssert(r == 0, "Function Did not return 0");
	customAssert(pre.numPlayers == post -> numPlayers, "Num Players Alterd");
	customAssert(pre.outpostPlayed == post->outpostPlayed, "OutpostPlayed Changed");
	customAssert(pre.outpostTurn == post -> outpostTurn, "OutpostTurn Changed" );
	customAssert(pre.whoseTurn == post -> whoseTurn, "WhosTurn Changed");
	customAssert(pre.phase == post -> phase, "Phase Changed");
	customAssert(pre.numActions == post -> numActions, "numActions Changed" );
	customAssert(pre.coins == post -> coins, "Coins Changed" );
	customAssert(pre.numBuys == post -> numBuys, "numBuys Changed" );
	
	int i =0;
	//test for same supply of cards 
	for(i=0; i< treasure_map;i++){
		customAssert(pre.supplyCount[i] == post->supplyCount[i],"Supply Off");
		customAssert(pre.embargoTokens[i] == post->embargoTokens[i], "embargo tokens failing");
	}
	
	//custom assert hand count the same
	customAssert( (pre.handCount[p] + 2) == post->handCount[p], "Hand Count not what expected\n");
		
	//assert hand updated correctly 
	for(i=0; i< pre.handCount[p];i++){
		customAssert( (pre.hand[p][i] == post->hand[p][i]),"failed handCount");	
	}		

	int firstNew = post->hand[p][post->handCount[p]-1];
	int secondNew = post->hand[p][post->handCount[p]-2];

	customAssert( (firstNew == copper || firstNew == silver || firstNew == gold), "added card not treasure");
	customAssert( (secondNew == copper || secondNew == silver || secondNew == gold), "added card not treasure");

	customAssert(pre.playedCardCount == post->playedCardCount, "played Card Count");	

	return 0;
}	

int main(int argc, char* argv[])
{
	srand(time(NULL));
	int i, n;

	struct gameState G;

 	for(n = 0; n < 200; n++){
		//randomize everything 
		for(i = 0; i < sizeof(struct gameState); i++){
			( (char*)&G)[i]=floor(Random()*256);
		}

		int p = 1;
		//set number players
		G.numPlayers= ( (rand() % (MAX_PLAYERS-2+1)) + 2);
	
		//set deckcount for player
		G.deckCount[p] = (rand() % (MAX_DECK - 7 + 1)) + 7;
		
		//set discard count
		G.discardCount[p] = rand() % MAX_DECK;
		
		//sethand count
		G.handCount[p] = (rand() %  (MAX_HAND-7+1))+7;
		
		//populate handcount 
		for(i = 0; i< G.deckCount[p];i++){
			G.deck[p][i] = rand() % 27;
		}
		
		//set deck cards 
		for( i = 0; i< G.deckCount[p]; i++){
			G.deck[p][i] = rand() % 27;
		}
		G.deck[p][0] = copper;
		G.deck[p][1] = copper; 

		//set discard cards
		for(i=0; i<G.discardCount[p]; i++){
			G.discard[p][i]= rand() % 27;
		}

		checkAdventurer(p,&G);
	}

	return 0;
}
