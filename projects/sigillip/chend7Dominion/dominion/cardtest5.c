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

void printHand(struct gameState testG, int thisPlayer){
	int i = 0;
	for(i=0;i<testG.handCount[thisPlayer];i++){
		printf("Card is %d\n",testG.hand[thisPlayer][i]);
		fflush(stdout);
	}
}

int main(int argc, char* argv[]){

        bool allTestPassed = true;

	printf("\n----------Testing Card 5, Feast-----------\n");        
        fflush(stdout);

        int seed = 1000;
        int numPlayers = 2;
        int thisPlayer = 0;
        int otherPlayer = 1;
        struct gameState G, testG;
        int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,sea_hag, tribute, smithy, council_room};
        int choice1 = 0;

	initializeGame(numPlayers, k, seed, &G);

	G.hand[thisPlayer][0] = copper;
        G.hand[thisPlayer][1] = gold;
        G.hand[thisPlayer][2] = duchy;
        G.hand[thisPlayer][3] = estate;
        G.hand[thisPlayer][4] = feast;

	//Test card added to hand 
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = village;
	
	printf("InitialHand:\n\n");
	fflush(stdout);
	printHand(testG, thisPlayer);
	
	cardEffect(feast, choice1, 0, 0, &testG, 0, 0);
	
	int i = 0;

	printf("Hand after Feast Played:\n\n");
	fflush(stdout);
	printHand(testG, thisPlayer);
	int villageCount = 0;
	for(i=0; i< testG.handCount[thisPlayer]; i++)
	{
		if(testG.hand[thisPlayer][i] == village){
			villageCount++;
		}
	}
	customAssert(villageCount == 1, "Card selected not added to hand",&allTestPassed);
	
	//test feast added to discardPile
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = village;
	
	int discardFeastCount = 0;
	for(i=0;i<testG.discardCount[thisPlayer];i++){
		if(testG.discard[thisPlayer][i] == feast){
			discardFeastCount++;
		}
	}
	cardEffect(feast, choice1, 0, 0, &testG, 0, 0);
	
	int postDiscardFeastCount = 0;
	for(i=0;i<testG.discardCount[thisPlayer];i++){
		if(testG.discard[thisPlayer][i] == feast){
			postDiscardFeastCount++;
		}
	}
	customAssert(discardFeastCount == (postDiscardFeastCount + 1 ), "Feast not added to Discard",&allTestPassed);

	//test wont allow selecting of card greater than 5 
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = gold;
	printf("Testing card too expensive, should say card too expensive:\n");
	int badCardResult = cardEffect(feast, choice1, 0, 0, &testG, 0, 0);
	customAssert(badCardResult == -1, "Didnt error out on selecting too expensive card",&allTestPassed);

	//test card selected out of stock 
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = village;
	testG.supplyCount[village] = 0;
	int cardOutOfStock = cardEffect(feast, choice1, 0, 0, &testG, 0, 0);
	customAssert(cardOutOfStock == -1, "Didnt error out on card being out of stock",&allTestPassed);
	
	//test supply of selected card decreased
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = village;
	testG.supplyCount[choice1] = 10;
	int expectedSupply = testG.supplyCount[choice1]-1;
	cardEffect(feast, choice1, 0, 0, &testG, 0, 0);
	customAssert(expectedSupply == testG.supplyCount[choice1], "Supply of selected valid card did not decrease by 1",&allTestPassed);
		
	//test count of selected card available decreases
	i = 0;
        for( i = 0; i < treasure_map; i++)
        {
                if(i != village)
                {
                        customAssert(testG.supplyCount[i] == G.supplyCount[i],"Kingdom cards altered", &allTestPassed);
                }
        }
	
	//test other players not impacted 
        for( i = 0; i < G.deckCount[otherPlayer]; i++)
        {
                customAssert(G.deck[otherPlayer][i] == testG.deck[otherPlayer][i], "Other player deck altered", &allTestPassed);
        }

	if(allTestPassed  == true )
        {
                printf("\nCARD TEST 5 PASSED\n");
                fflush(stdout);
        }
        else
        {
                printf("\nCARD TEST 5 FAILED\n");
                fflush(stdout);
        }

        return 0;
}

