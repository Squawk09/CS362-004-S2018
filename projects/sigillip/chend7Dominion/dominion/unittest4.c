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

        printf("\n----------Testing Func 4, playCard--------\n");
        fflush(stdout);

        int seed = 1000;
        int numPlayers = 2;
        int thisPlayer = 0;
        struct gameState G, testG;
        int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,sea_hag, tribute, smithy, council_room};       

	G.hand[thisPlayer][0] = copper;
	G.hand[thisPlayer][1] = gold;
	G.hand[thisPlayer][2] = duchy;
	G.hand[thisPlayer][3] = estate;
	G.hand[thisPlayer][4] = village;

	int testResult = -1;

	initializeGame(numPlayers, k, seed, &G);
	
	//test if phase incorrect 
        memcpy(&testG, &G, sizeof(struct gameState));
        testG.phase = -1;
	testG.numActions = 1;
	testResult = playCard(4,0,0,0, &testG);
	customAssert(testResult == -1, "Should have failed play card, not right phase",&allTestPassed);

	//test if num actions less than 1 
	memcpy(&testG, &G, sizeof(struct gameState));
        testG.phase = 0;
	testG.numActions = 0;
	testResult = playCard(4,0,0,0, &testG);
	customAssert(testResult == -1, "Should have failed play card, not enough actions",&allTestPassed);

	//check if player tries to play non action card 
	memcpy(&testG, &G, sizeof(struct gameState));
        testG.phase = 0;
	testG.numActions = 1;
	testResult = playCard(1,0,0,0, &testG);
	customAssert(testResult == -1, "Should have failed play card, not action card selected",&allTestPassed);
	
	//test normal play 
	memcpy(&testG, &G, sizeof(struct gameState));
  	testG.hand[thisPlayer][0] = village;
	testG.phase = 0;
	testResult = playCard(0,0,0,0,&testG);
	customAssert(testResult == 0, "Should have been able to play card",&allTestPassed);
	
	if(allTestPassed  == true )
        {
                printf("\nTESTING FUNCTION 4 PASSED\n");
                fflush(stdout);
        }
        else
        {
                printf("\nTESTING FUNCTION 4 TEST FAILED\n");
                fflush(stdout);
        }

	return 0;
}


