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

int checkCouncilRoom(int p, struct gameState *post) {
	

	struct gameState pre;
	memcpy(&pre,post,sizeof(struct gameState));
	
	play_council_room(post, 0);  
	
	//+4 other players draw a card 
	int i = 0;
	for(i=0; i< post->numPlayers; i++){
		drawCard(i, &pre);
	}
	
	//setup precard to compare against
	drawCard(p, &pre);
	drawCard(p, &pre);
	drawCard(p, &pre);
	discardCard(0, p, &pre, 0);
	pre.numBuys++;
		
	if( (memcmp(&pre, post, sizeof(struct gameState)) != 0) )
	{
		customAssert(pre.numPlayers == post -> numPlayers, "Num Players Alterd");
	        customAssert(pre.outpostPlayed == post->outpostPlayed, "OutpostPlayed Changed");
        	customAssert(pre.outpostTurn == post -> outpostTurn, "OutpostTurn Changed" );
        	customAssert(pre.whoseTurn == post -> whoseTurn, "WhosTurn Changed");
        	customAssert(pre.phase == post -> phase, "Phase Changed");
        	customAssert(pre.numActions == post -> numActions, "numActions Changed" );
        	customAssert(pre.coins == post -> coins, "Coins Changed" );
        	customAssert(pre.numBuys == post -> numBuys, "numBuys incorrect" );
	        customAssert( (pre.handCount[p] + 3) == post->handCount[p], "PlayerHand Count not what expected\n");
		for(i=0; i < pre.numPlayers; i++){
			if(i != p){
				if(pre.handCount[i] != post->handCount[i])
				{
					printf("Assertion Failed:Player %d had incorrect number of cards\n",i);
				}
			}
		}
			
	}

	return 0;
}	

int main(int argc, char* argv[])
{

	printf("\n\n----------\nTesting CouncilRoom with random values\n----------\n\n");
	fflush(stdout);

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
		G.numPlayers= (rand() % (MAX_PLAYERS - 2 + 1 )) + 2;
		G.whoseTurn = 0;		
		//set deckcount for player
		int j=0;
		for(j=0; j<G.numPlayers; j++){

			
			G.deckCount[j] = (rand() % (MAX_DECK - 100 + 1)) + 100;
			
			//set discard count
			G.discardCount[j] = rand() % MAX_DECK;
			
			//sethand count
			G.handCount[j] = 10;
		
			//populate handcount for each player
			for(i = 0; i< G.handCount[j];i++){
				G.hand[j][i] = rand() % 27;
			}
					
			//set deck cards for each player
			for( i = 0; i< G.deckCount[j]; i++){
				G.deck[j][i] = rand() % 27;
			}
			
			//set discard cards
			for(i=0; i<G.discardCount[j]; i++){
				G.discard[j][i]= rand() % 27;
			}

			G.playedCardCount = 0;

		}
						checkCouncilRoom(p,&G);
	}
	printf("\n\n----------\nFinished Testing CouncilRoom Smithy\n----------\n\n");
	return 0;
}
