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

        printf("\n----------Testing Func 1, supplyCount-----\n");
        fflush(stdout);

        int seed = 1000;
        int numPlayers = 2;
        struct gameState G, testG;
        int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,sea_hag, tribute, smithy, council_room};
        
	initializeGame(numPlayers, k, seed, &G);
	
	//test for all cards
	int i = 0;
	for(i = 0; i <= treasure_map; i++){

	        testG.supplyCount[i] = 10;
		
		//test return positive number 
		int result = supplyCount(i,&testG);
		customAssert(result == testG.supplyCount[i],"Did not return positive value expected",&allTestPassed);

		//test negative value
		testG.supplyCount[i] = -10;
		result = supplyCount(i,&testG);
		customAssert(result == testG.supplyCount[i],"Did not return negative value expected",&allTestPassed);
	
		//test zero
		testG.supplyCount[i] = 0;
		result = supplyCount(i,&testG);
		customAssert(result == testG.supplyCount[i],"Did not return zero",&allTestPassed);
	}

        if(allTestPassed  == true )
        {
                printf("\nTESTING FUNCTION 1 PASSED\n");
                fflush(stdout);
        }
        else
        {
                printf("\nTESTING FUNCTION 1 FAILED\n");
                fflush(stdout);
        }

        return 0;
}

