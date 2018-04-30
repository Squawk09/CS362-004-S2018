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

        printf("\n----------Testing Func 2, IsGameOver------\n");
        fflush(stdout);

        int seed = 1000;
        int numPlayers = 2;
        struct gameState G, testG;
        int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,sea_hag, tribute, smithy, council_room};
        int result = -1;

        initializeGame(numPlayers, k, seed, &G);

	//if provice is not empty dont end game
	memcpy(&testG, &G, sizeof(struct gameState));
	result = isGameOver(&testG);
	customAssert(result == 0, "Game should not have ended, provice cards remain", &allTestPassed);

	//test game should end 
	testG.supplyCount[province] = 0;
	result = isGameOver(&testG);
	customAssert(result == 1, "Game Should have ended, province cards gone", &allTestPassed);

	//test for all combinations of cards where three are empty 
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.supplyCount[province] = 10;
	int i,j,q = 0;
	for(i=0; i < 8; i++)
	{
		for(j=i+1; j < 9;j++)
		{
			for(q = j+1; q<10; q++)
			{
				//test 3 piles empty
				testG.supplyCount[q] = 0;
				testG.supplyCount[j] = 0;
				testG.supplyCount[i] = 0;
				result = isGameOver(&testG);
				customAssert(result == 1, "Game Should have ended, three piles empty",&allTestPassed);
				
				testG.supplyCount[q] = 10;
				testG.supplyCount[j] = 10;
				testG.supplyCount[i] = 10;
			}	
		}
	}
	
	if(allTestPassed  == true )
        {
                printf("\nFUNCTION TEST 2 PASSED\n");
                fflush(stdout);
        }
        else
        {
                printf("\nFUNCTION TEST 2 FAILED\n");
                fflush(stdout);
        }

	return 0;
}


