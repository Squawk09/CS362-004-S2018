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

int checkSmithy(int p, struct gameState *post) {

	struct gameState pre;
	memcpy(&pre,post,sizeof(struct gameState));
	
	play_smithy(post, 0);

	//setup pre to compare against
	drawCard(p, &pre);
	drawCard(p, &pre);
	drawCard(p, &pre);
	discardCard(0, p, &pre, 0);

	customAssert(memcmp(&pre, post, sizeof(struct gameState)) == 0, "Game State Invalidly Altered");
	return 0;
}	

int main(int argc, char* argv[])
{

	printf("\n\n----------\nTesting Smithy with random values\n----------\n\n");
	fflush(stdout);

	srand(time(NULL));
	int i, n;

 	struct gameState G;

 	for(n = 0; n < 200; n++){
		//randomize everything 
		for(i = 0; i < sizeof(struct gameState); i++){
			( (char*)&G)[i]=floor(Random()*256);
		}

		int p = 0;
		//set number players
		G.numPlayers= (rand() % MAX_PLAYERS);
	
		//set deckcount for player
		G.deckCount[p] = (rand() % (50 - 7 + 1)) + 7;
		
		//set discard count
		G.discardCount[p] = rand() % MAX_DECK;
		
		//sethand count
		G.handCount[p] = rand() %  MAX_HAND;
		
		//populate handcount 
		for(i = 0; i< G.handCount[p];i++){
			G.hand[p][i] = rand() % 27;
		}

		//set deck cards 
		for( i = 0; i< G.deckCount[p]; i++){
			G.deck[p][i] = rand() % 27;
		}
		
		//set discard cards
		for(i=0; i<G.discardCount[p]; i++){
			G.discard[p][i]= rand() % 27;
		}

		G.playedCardCount = 0;
		G.whoseTurn = 0;
		checkSmithy(p,&G);
	}
	printf("\n\n----------\nFinished Testing Smithy\n----------\n\n");
	return 0;
}
