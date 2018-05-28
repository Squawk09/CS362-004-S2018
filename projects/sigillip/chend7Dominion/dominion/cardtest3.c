#include<stdio.h>
#include<stdlib.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

void customAssert(int expression, char* errorMessage, bool *allPassed){

        if(expression != 1){
                printf("Assertion Failed: %s\n",errorMessage);
                fflush(stdout);
                *allPassed = false;
        }
}

int main(int argc, char* argv[]){

        bool allTestPassed = true;

        printf("\n----------Testing Card 3, Council Room----\n");
	fflush(stdout);

        int seed = 1000;
        int numPlayers = 2;
        int thisPlayer = 0;
        struct gameState G, testG;
        int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,sea_hag, tribute, smithy, council_room};

        initializeGame(numPlayers, k, seed, &G);

	//Test playing card returns 0
	memcpy(&testG, &G, sizeof(struct gameState));
	int expectedHandCount = testG.handCount[thisPlayer]+3;
	int expectedBuyCount = testG.numBuys + 1;
	int result = cardEffect(council_room, 0, 0, 0, &testG, 0, 0);
	customAssert(result == 0, "Should return 0 as value", &allTestPassed);
	
	//Test player hand increases by 4 
	customAssert(expectedHandCount == testG.handCount[thisPlayer],"Player didnt receive 3 cards", &allTestPassed);

	//Test Buy Count increases once for player
	customAssert(expectedBuyCount == testG.numBuys,"Player Buying Power did not increase", &allTestPassed);
	
	//Test other players hands increase by 1
	int otherPlayer = 1;
	cardEffect(council_room, 0, 0, 0, &testG, 0, 0);
	customAssert(testG.handCount[otherPlayer] == 1,"Other players did not draw card", &allTestPassed);

	//other players draw from their own deck and draws top card of deck
	memcpy(&testG, &G, sizeof(struct gameState));
	int expectedCard = testG.deck[otherPlayer][G.deckCount[otherPlayer]-1];
	cardEffect(council_room, 0, 0, 0, &testG, 0, 0);
	customAssert(expectedCard == testG.hand[otherPlayer][testG.handCount[otherPlayer]-1],"Other players didnt draw from top of their deck",&allTestPassed);

	customAssert(G.discardCount[otherPlayer] == testG.discardCount[otherPlayer],"Other player discard count changed", &allTestPassed);

	//victory and kingdom cards dont change
	memcpy(&testG, &G, sizeof(struct gameState));
	cardEffect(council_room, 0, 0, 0, &testG, 0, 0);
	int i = 0;
	for( i = 0; i < treasure_map; i++)
	{
		customAssert(testG.supplyCount[i] == G.supplyCount[i],"Kingdom cards altered", &allTestPassed);
	}
        
        if(allTestPassed  == true )
        {
                printf("\nCARD 3 TEST PASSED\n");
                fflush(stdout);
        }
        else
        {
                printf("\nCARD 3 TEST FAILED\n");
                fflush(stdout);
        }

        return 0;
}

