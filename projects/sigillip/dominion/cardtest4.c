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

        if(expression != 1)
	{
                printf("Assertion Failed: %s\n",errorMessage);
                fflush(stdout);
                *allPassed = false;
        }
}


int main(int argc, char* argv[]){

        bool allTestPassed = true;

        printf("\n----------Testing Card 4, Mine------------\n");
        fflush(stdout);

        int seed = 1000;
        int numPlayers = 2;
        int thisPlayer = 0;
        int otherPlayer = 1;
	struct gameState G, testG;
        int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,sea_hag, tribute, smithy, council_room};
	int choice1, choice2 = 0;

        initializeGame(numPlayers, k, seed, &G);

	G.hand[thisPlayer][0] = copper;
	G.hand[thisPlayer][1] = gold;
	G.hand[thisPlayer][2] = duchy;
	G.hand[thisPlayer][3] = estate;
	G.hand[thisPlayer][4] = feast;

	//Test player not allowed to trash non treasure card
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.hand[thisPlayer][0] = village;
	choice1 = 0;
	choice2 = gold;
	int result = cardEffect(mine, choice1, choice2, 0, &testG, 0, 0);	 	
	customAssert(result == -1,"Allowed player to trash non-treasury card", &allTestPassed);

	//Test player not allowed to take non treasure card
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.hand[thisPlayer][0] = gold;
	choice1 = 0;
	choice2 = feast;
	result = cardEffect(mine, choice1, choice2, 0, &testG, 1, 0);
	customAssert(result == -1,"Allowed player to take non-treasury card", &allTestPassed);

	//Test Player cannot swap for card valued greater than +3 
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.hand[thisPlayer][0] = copper;
	choice1 = 0;
	choice2 = gold;
	result = cardEffect(mine, choice1, choice2, 0, &testG, 0, 0);
	customAssert(result != -1,"Allowed player to take too valuable card", &allTestPassed);

	//Test Player can swap treasure card for treasure card valued <= card + 3
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.hand[thisPlayer][0] = copper;
	choice1 = 0;
	choice2 = silver;
	cardEffect(mine, choice1, choice2, 0, &testG, 0, 0);
	customAssert(testG.hand[thisPlayer][0] == silver,"Player did not get selected card", &allTestPassed);

	//count of treasury card selected decreases by 1 
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.hand[thisPlayer][0] = copper;
	choice1 = 0;
	choice2 = silver;
	int expectedCount = testG.supplyCount[silver]-1;
	cardEffect(mine, choice1, choice2, 0, &testG, 0, 0);
	customAssert(expectedCount == testG.supplyCount[silver],"Selected treasury card count did not decrease", &allTestPassed);

	//Test treasury and kingdom cards not selected do not change 
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 0;
	choice2 = silver;
	cardEffect(mine, choice1, choice2, 0, &testG, 0, 0);

	int i = 0;
        for( i = 0; i < treasure_map; i++)
        {
		if(i != silver)
		{
	                customAssert(testG.supplyCount[i] == G.supplyCount[i],"Kingdom cards altered", &allTestPassed);
		}
	}
	
	//Test other player decks do not change 
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 0;
	choice2 = silver;
	cardEffect(mine, choice1, choice2, 0, &testG, 0, 0);

	for( i = 0; i < G.deckCount[otherPlayer]; i++)
	{
		customAssert(G.deck[otherPlayer][i] == testG.deck[otherPlayer][i], "Other player deck altered", &allTestPassed);
	}

	//Test selected card sent to trash
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.hand[thisPlayer][0] = silver;
	testG.hand[thisPlayer][1] = mine;
	choice1 = 0;
	choice2 = gold;
	int expectedTrashCount = testG.discardCount[thisPlayer]+2;
	cardEffect(mine, choice1, choice2, 0, &testG, 1, 0);
	customAssert(testG.discardCount[thisPlayer] == expectedTrashCount, "Incorrect number of cards trashed", &allTestPassed);

	//Test card sent to trash was card selected 
	customAssert(testG.discard[thisPlayer][testG.discardCount[thisPlayer]-1] == silver, "Most Recent Card sent to trash not card selected", &allTestPassed);

	//Test card added to hand was card selected 
	customAssert(testG.hand[thisPlayer][0] == silver, "Card selected not added to hand", &allTestPassed);
	
        if(allTestPassed  == true )
        {
                printf("\nCARD 4 TEST PASSED\n");
                fflush(stdout);
        }
        else
        {
                printf("\nCARD 4 TEST FAILED\n");
                fflush(stdout);
        }

	return 0;
}
