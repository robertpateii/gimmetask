#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// https://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)Randomization.html

#define MAXTASKS 100
// 80 characters, 2 completed/deleted marker and trailing space,  1 for end of line
#define MAXTASKLEN 83
char deleted = '-';
char completed = 'x';

char tasks[MAXTASKS][MAXTASKLEN];

enum action {
	Exit = 0,
	Complete,
	Skip,
	Delete,
	Add
};

void printAllTasks() {
	for (int i = 0; i < 100; i++) {
		if (strlen(tasks[i]) > 0) {
			printf("task #%d: %s",i+1, tasks[i]); // tasks have their own linebreak
		}
	}
}

void addTasks() {
	
	// during testing ensure we have some tests tasks w/o having to enter them
	strcpy(tasks[0], "Email replies\n");
	strcpy(tasks[1], "Short study/research\n");

	char tempTask[81];
	printf("Enter tasks, 80 char max, type done to stop\n");
	int i = 2; // go back to 0 once test tasks are removed above
	char* doneStr = "done\n";
	while (fgets (tempTask, 81, stdin) != NULL) {
		if (strcmp(tempTask, doneStr)) { // 0 is match
			printf("Got: %s", tempTask);
			strcpy(tasks[i], tempTask);
			i++;
		} else {
			break;
		}
	}

	printf("done taking tasks, now printing all:\n");
	printAllTasks();
}

void addMore() {

	// if we never remove tasks with delete/complete, we can use taskCount+1 as the new task position
	// do we want to let the user add multiple at once? or just send them back to the menu?
	printf("Not ready yet, so have a list of all the tasks:\n");
	printAllTasks();
}

int countTasks() {
	int count = 0;
	for (int i = 0; i < 100; i++) {
		// use strlen not sizeof because size is fixed not dynamic
		if (strlen(tasks[i]) > 0) {
			count++;
		}
	}
	return count;
}

int getTask(int taskCount) {
	int randTask = rand() % taskCount;
	int attempts = 0;
	while(tasks[randTask][0] == deleted || tasks[randTask][0] == completed) {
		// got deleted or completed task, so find a new one
		randTask = rand() % taskCount;
		attempts++;
		if (attempts > 10) {
			printf("Too many deleted/completed tasks, found 10 in a row\n");
			printf("Warning: leaving the current task as -1 position, you should exit.\n");
			return -1;
		}
	}
	printf("Do #%d next:\n", randTask+1);
	printf("%s",tasks[randTask]);
	return randTask;
}

enum action getNextAction() {
	// I probably will have leftover EOF in stdin from inputting tasks
	// https://sekrit.de/webdocs/c/beginners-guide-away-from-scanf.html
	// https://stackoverflow.com/questions/30304368/end-while-loop-with-ctrld-scanf
	char x[10];
	printf("Complete, Skip, Delete, Add, or Exit?\n");
	if (fgets (x, 10, stdin) != NULL) {
		*x = tolower(*x);	
		if (*x == 'c') return Complete;
		if (*x == 's') return Skip;
		if (*x == 'd') return Delete;
		if (*x == 'a') return Add;
		if (*x == 'e') return Exit;
		// default
		printf("no match, exiting, got:\n");
		printf("|%s|\n",x);
		return Exit;
	} else {
		printf("got null/eof, exiting.\n");
		return Exit;
	}
}

void prependTaskPrep(int pos) {
	char *temp = strdup(tasks[pos]);
	for (int i = MAXTASKLEN; i > 1; i--) {
		// we should have 2 characters free at the end
		// so copy everything higher by 2
		// then we'll put "- " at the front
		// 83 = 81
		// 82 = 80
		// ...
		// 2 = 0
		// stop
		temp[i] = tasks[pos][i-2];
	}
	strcpy(tasks[pos], temp);
	free(temp);
}

void delete(int pos) {
	if (pos == -1) {
		printf("Error: -1 current task, you should exit.\n");
		return;
	}
	prependTaskPrep(pos);
	tasks[pos][0] = deleted;
	tasks[pos][1] = ' ';
}

void complete(int pos) {
	if (pos == -1) {
		printf("Error: -1 current task, you should exit.\n");
		return;
	}
	prependTaskPrep(pos);
	tasks[pos][0] = completed;
	tasks[pos][1] = ' ';
}

int main() {
	srand(time(0));
	addTasks(); // operates on global tasks
	int length = countTasks();
	printf("Max tasks: %d; Current tasks: %d\n", MAXTASKS, length);
	int currTaskPos = getTask(length); // also prints the task
	enum action nextAction;
	while (nextAction = getNextAction()) { // 0 is exit
		switch (nextAction) {
		case 1:
			printf("Got Complete\n");
			complete(currTaskPos);
			printf("Completed, getting new one:\n");
			currTaskPos = getTask(length);
			break;
		case 2:
			printf("Got Skip\n");
			currTaskPos = getTask(length);
			break;
		case 3:
			printf("Got Delete for task #%d\n", currTaskPos);
			// no real different than complete  as long as everything is in ram
			// exclude task from being randomly selected, or shown in lists if that is added
			// so strpy a blank string on it?
			delete(currTaskPos);
			printf("Deleted, getting new one:\n");
			currTaskPos = getTask(length);
			break;
		case 4:
			printf("Got Add\n");
			addMore();
			break;
		default:
			printf("Got unexpected\n");
			break;
		}
	}
	return 0;
}
