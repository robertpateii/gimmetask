#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
// https://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)Randomization.html

#define MAXTASKS 100
char *tasks[MAXTASKS];

int initTasks() {
	// init the array with strings so we can use strlen to see if one is empty
	for (int i = 0; i < 100; i++) {
		tasks[i] = "";
	}
	return 0;
}

int addTasks() {
	tasks[0] = "test task 0\n";
	tasks[1] = "test task 1\n";
	tasks[2] = "test task 2\n";
	tasks[3] = "test task 3\n";
}

int countTasks() {
	int count = 0;
	for (int i = 0; i < 100; i++) {
		if (strlen(tasks[i]) > 0) {
			count++;
		}
	}
	return count;
}

int getTask(int taskCount) {
	int randTask = rand() % taskCount;
	printf("%d rand task num\n",randTask);
	return randTask;
}

int askUserNextStep() {
	char name[2];
	char c;
	int error = 1;
	if (scanf("%1[^\n]%*c", name) == 1) {
		c = name[0];
		switch (c) {
			case 'c':
			case 'C':
			case 's':
			case 'S':
				int length = countTasks();
				int currTask = getTask(length);
				printf(tasks[currTask]);
				error = 0;
				break;
			case 'd':
			case 'D':
			case 'e':
			case 'E':
				printf("doing %c\n", c);
				error = 0;
				break;
			default:
				printf("Input C, S, D, or E please.\n");
				error = 1;
				break;
		}
	} else {
		printf("Input C, S, D, or E please.\n");
		error = 1;
	}
	return error;
}

int main() {
	srand(time(0));
	initTasks(); // inits golbal tasks
	addTasks(); // operates on global tasks
	int length = countTasks();
	printf("Max tasks: %d \n current tasks: %d\n", MAXTASKS, length);
	int currTask = getTask(length);
	printf(tasks[currTask]);
	// here's where we scanf and process some user input
	// will they complete the task, skip the task, delete the task?
	printf("Complete, Skip, Delete, or Exit?\n");
	// getting this to loop is next, maybe
	// https://sekrit.de/webdocs/c/beginners-guide-away-from-scanf.html
	return askUserNextStep();
}
