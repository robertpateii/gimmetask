#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// https://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)Randomization.html

#define MAXTASKS 100
#define MAXTASKLEN 81

char tasks[MAXTASKS][MAXTASKLEN];

enum action {
	Exit = 0,
	Complete,
	Skip,
	Delete,
	Add
};

void addTasks() {
	
	// during testing ensure we have some tests tasks w/o having to enter them
	strcpy(tasks[0], "test task #1\n");
	strcpy(tasks[1], "test task #2\n");

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

	for (int i = 0; i < 100; i++) {
		if (strlen(tasks[i]) > 0) {
			printf("task #%d: %s",i+1, tasks[i]); // tasks have their own linebreak
		}

	}

}

void addMore() {
	// find first empty string in array (if none, bail)
	// get new task
	// add to the empty slot
	// send back to nextAction()?
	// could allow for adding many at once here, but would that be needed much? would need to say done everytime you add one or something instead of just a blah blah enter
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

int main() {
	srand(time(0));
	addTasks(); // operates on global tasks
	int length = countTasks();
	printf("Max tasks: %d; Current tasks: %d\n", MAXTASKS, length);
	int currTaskPos = getTask(length);
	printf("Do #%d next:\n", currTaskPos+1);
	printf(tasks[currTaskPos]);
	enum action nextAction;
	while (nextAction = getNextAction()) { // 0 is exit
		switch (nextAction) {
		case 1:
			printf("Got Complete\n");
			// no real different than delete as long as everything is in ram
			// exclude task from being randomly selected, or shown in lists if that is added
			// so strpy a blank string on it?
			break;
		case 2:
			printf("Got Skip\n");
			// should prob be its own function since doing it at least twice 
			// also it gets a random number within the task count
			// but then uses it as the position. If I start adding/removing it
			// will get way off.
			currTaskPos = getTask(length);
			printf("Do #%d next:\n", currTaskPos+1);
			printf(tasks[currTaskPos]);
			break;
		case 3:
			printf("Got Delete\n");
			// no real different than complete  as long as everything is in ram
			// exclude task from being randomly selected, or shown in lists if that is added
			// so strpy a blank string on it?
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
