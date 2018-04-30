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

int main(int argc, char *argv[]){

	int seed = 1000;
	int numPlayers = 2;
	int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,sea_hag, tribute, smithy, council_room};
	bool allTestPassed = true;

	initializeGame(numPlayers, k, seed, &G);
	
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n----------Testing Card 1, Smithy----------\n");
	fflush(stdout);

	//Test player received 2 extra cards
	int expectedHandCount = testG.handCount[thisPlayer] + 3 - 1;
	cardEffect(smithy, 0, 0, 0, &testG, 0, 0);
	customAssert(testG.handCount[thisPlayer] == expectedHandCount,"Hand Count Incorrect", &allTestPassed);

	//Test playing card returns 0 
	memcpy(&testG, &G, sizeof(struct gameState));
	int result = cardEffect(smithy, 0, 0, 0, &testG, 0, 0);
	customAssert(result == 0, "Should return 0 as value", &allTestPassed);
	
	//Test Cards come fron own pile 
	memcpy(&testG, &G, sizeof(struct gameState));
	cardEffect(smithy, 0, 0, 0, &testG, 0, 0);
	customAssert( testG.deckCount[thisPlayer] == G.deckCount[thisPlayer] - 3, "Card didnt come from own deck", &allTestPassed);
	
	//Test Other player states dont change 
	memcpy(&testG, &G, sizeof(struct gameState));
	int otherPlayer = 1;
	cardEffect(smithy, 0, 0, 0, &testG, 0, 0);
	
	customAssert(G.deckCount[otherPlayer] == testG.deckCount[otherPlayer],"Other player deck count changed", &allTestPassed);
	customAssert(G.discardCount[otherPlayer] == testG.discardCount[otherPlayer],"Other player discard count changed", &allTestPassed);
	
	int i = 0;
	for( i = 0; i < G.deckCount[otherPlayer]; i++)
	{
		customAssert(G.deck[otherPlayer][i] == testG.deck[otherPlayer][i], "Other player deck altered", &allTestPassed);
	}
	
	//victory and kingdom cards dont change 
	memcpy(&testG, &G, sizeof(struct gameState));
	cardEffect(smithy, 0, 0, 0, &testG, 0, 0);
	for( i = 0; i < treasure_map; i++){
		customAssert(testG.supplyCount[i] == G.supplyCount[i],"Kingdom cards altered", &allTestPassed);
	}

        if(allTestPassed  == true )
        {
                printf("\n CARD 1 TEST PASSED\n");
                fflush(stdout);
        }
        else
        {
                printf("\nCARD 1 TEST FAILED\n");
                fflush(stdout);
        }

	return 0;
}	
