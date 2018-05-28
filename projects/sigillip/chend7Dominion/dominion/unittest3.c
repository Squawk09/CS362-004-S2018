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

	printf("\n----------Testing Func 3, BuyCard---------\n");
        fflush(stdout);

        int seed = 1000;
        int numPlayers = 2;
        int thisPlayer = 0;
        struct gameState G, testG;
        int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,sea_hag, tribute, smithy, council_room};
        
        initializeGame(numPlayers, k, seed, &G);

	int testResult = -1;
	//test card not in stock 
        memcpy(&testG, &G, sizeof(struct gameState));
	testG.numBuys = 1;
	testG.coins = 100;
	testG.supplyCount[village] = 0;
	testResult = buyCard(village, &testG);
	customAssert(testResult == -1, "Card is out of stock, should have failed buy", &allTestPassed);

	//test card is in stock 
	testG.supplyCount[village] = 10;
	testResult = buyCard(village, &testG);
	customAssert(testResult == 0, "Card is in stock, should have succeeded buy", &allTestPassed);

	//test player does not have enough buys 
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.numBuys = 0;
	testG.coins = 100;
	testG.supplyCount[village] = 10;
	testResult = buyCard(village, &testG);
	customAssert(testResult == -1, "Player out of buys, should have failed ", &allTestPassed);

	//test number of buys decreases 
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.numBuys =10;
	testG.coins = 100;
	testG.supplyCount[village] = 10;
	testResult = buyCard(village, &testG);
	customAssert(testG.numBuys == 9, "Buys should have decreased by 1", &allTestPassed);

	//test coins decrease by price 
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.numBuys =10;
	testG.coins = 100;
	testG.supplyCount[village] = 10;
	testResult = buyCard(village, &testG);
	customAssert(testG.coins == 100 - getCost(village), "Buy should have decreased coins by cost", &allTestPassed);

	//test card goes into discard 
	customAssert(testG.discard[thisPlayer][testG.discardCount[thisPlayer-1]] == village,"Card should have been added to discard", &allTestPassed); 

        if(allTestPassed  == true )
        {
                printf("\nTESTING FUNCTION 3 PASSED\n");
                fflush(stdout);
        }
        else
        {
                printf("\nTESTING FUNCTION 3 FAILED\n");
                fflush(stdout);
        }

        return 0;
}

