#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Add points on task complete
Add points on time passed while open
Spend points increasing above or buying levels
Level cost increases

*/

static int points;
static int timeFactor;
static int taskFactor;
static int level;

int getPoints() {
    return points;
}

int setPoints(int delta) {
    // TODO: points should be unsigned but don't let them go negative here
    points += delta;
    return getPoints();
}

int timeBump() {
    setPoints(level * timeFactor);
}

int taskBump() {
    setPoints(level * taskFactor);
}

void exportPoints() {
	FILE *filePtr;
	filePtr = fopen("points.txt","w");

	if (filePtr == NULL) {
		printf("Could not open points file for writing.\n");
		return;
	}
    
    // TODO: save points, factors, and levels

	fclose(filePtr);
}


// 1 means no backup file found, 0 means import success
int importPoints() {
	// Assumes only run at start
	FILE *importFilePtr;
	importFilePtr = fopen("points.txt", "r");

	if (importFilePtr == NULL) {
		printf("No points file.\n");
		return 1;
	}

    // TODO: read tasks, factors, and levels

	fclose(importFilePtr);

	printf("** Points Import Successful **\n");
	return 0;
}
