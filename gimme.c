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
	// do not re-order unless you change the switch in main
	Exit = 0,
	Complete,
	Skip,
	Delete,
	Add,
	List,
	Retry,
	Get,
	Export
};

void printAllTasks() {
	for (int i = 0; i < MAXTASKS; i++) {
		if (strlen(tasks[i]) > 0) {
			printf("#%d: %s",i+1, tasks[i]); // tasks have their own linebreak
		}
	}
}


void exportTasks() {
	for (int i = 0; i < MAXTASKS; i++) {
		if (strlen(tasks[i]) > 0 && tasks[i][0] != deleted && tasks[i][0] != completed ) {
			printf("%s", tasks[i]); // tasks have their own linebreak
		}
	}
}

void addTasks(int startPos) {
	

	char tempTask[MAXTASKLEN-2];
	printf("Enter tasks, 80 char max, type done to stop\n");
	int i = startPos;
	char* doneStr = "done\n";
	while (fgets (tempTask, MAXTASKLEN-2, stdin) != NULL) {
		if (strcmp(tempTask, doneStr)) { // 0 is match
			printf("Got: %s", tempTask);
			strcpy(tasks[i], tempTask);
			i++;
		} else {
			break;
		}
	}

	printf("** New list **\n");
	printAllTasks();
}

int countTasks() {
	int count = 0;
	for (int i = 0; i < MAXTASKS; i++) {
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
		if (attempts > 20) {
			printf("Error: Found 20 deleted/completed tasks in a row. Are any valid?\n");
			return -1;
		}
	}
	printf("#%d: %s", randTask+1, tasks[randTask]);
	return randTask;
}

int getSpecificTask() {
	char x[10];
	char* end;
	printf("What task number do you want?\n");
	if (fgets (x, 10, stdin) != NULL) {
		int t;
		t = strtol(x, &end, 10);
		if (0 < t < MAXTASKS+1) {
			printf("#%d: %s", t, tasks[t-1]);
			return t-1;
		} else {
			printf("Task num out of bounds, doing nothing\n");
		}
	} else {
		printf("Unexpecetd null/eof, doing nothing.\n");
	}
	// fail state
	return -1;

}

enum action getNextAction() {
	// I probably will have leftover EOF in stdin from inputting tasks
	// https://sekrit.de/webdocs/c/beginners-guide-away-from-scanf.html
	// https://stackoverflow.com/questions/30304368/end-while-loop-with-ctrld-scanf
	char x[10];
	printf("Complete, Skip, Delete, Add, List, Get, or Exit?\n");
	if (fgets (x, 10, stdin) != NULL) {
		*x = tolower(*x);	
		if (*x == 'c') return Complete; // how about done?
		if (*x == 's') return Skip; // how about next?
		if (*x == 'd') return Delete; // how about remove?
		if (*x == 'a') return Add;
		if (*x == 'l') return List;
		if (*x == 'n') return Skip; // easter egg for vim users
		if (*x == 'g') return Get;
		if (*x == 'e') return Exit;// how about quit?
		if (*x == 'x') return Export;// how about export?
		// default
		printf("Invalid input, try again.\n");
		return Retry;
	} else {
		printf("Unexpecetd null/eof, exiting.\n");
		return Exit;
	}
}

void prependTaskPrep(int pos) {
	char temp[MAXTASKLEN];
	char temp2[MAXTASKLEN] = "  ";
	strcpy(temp, tasks[pos]);
	strcat(temp2, temp);
	strcpy(tasks[pos], temp2);;
}

void delete(int pos) {
	if (pos == -1) {
		printf("Error: No current task, are any available?\n");
		return;
	}
	prependTaskPrep(pos);
	tasks[pos][0] = deleted;
	tasks[pos][1] = ' ';
}

void complete(int pos) {
	if (pos == -1) {
		printf("Error: No current task, are any available?\n");
		return;
	}
	prependTaskPrep(pos);
	tasks[pos][0] = completed;
	tasks[pos][1] = ' ';
}

int main() {
	srand(time(0));// for getTask which should be randomish
	addTasks(0);
	int length = countTasks();
	printf("Max tasks: %d; Total tasks: %d\n", MAXTASKS, countTasks());
	printf("Next is ");
	int currTaskPos = getTask(length); // also prints the task
	enum action nextAction;
	while (nextAction = getNextAction()) { // 0 is exit
		length = countTasks();
		switch (nextAction) {
		case 1:
			printf("Done w/ #%d: %s", currTaskPos+1, tasks[currTaskPos]);
			complete(currTaskPos);
			printf("Next is ");
			currTaskPos = getTask(length);
			break;
		case 2:
			currTaskPos = getTask(length);
			break;
		case 3:
			printf("Deleted #%d: %s", currTaskPos+1, tasks[currTaskPos]);
			delete(currTaskPos);
			printf("Next is ");
			currTaskPos = getTask(length);
			break;
		case 4:
			addTasks(countTasks());
			printf("Current #%d: %s", currTaskPos+1, tasks[currTaskPos]);
			break;
		case 5:
			printAllTasks();
			printf("Current #%d: %s", currTaskPos+1, tasks[currTaskPos]);
			break;
		case 6:
			printf("Current #%d: %s", currTaskPos+1, tasks[currTaskPos]);
			break;
		case 7:
			currTaskPos = getSpecificTask();
			break;
		case 8:
			exportTasks();
			printf("Current #%d: %s", currTaskPos+1, tasks[currTaskPos]);
			break;
		default:
			printf("Got unexpected action!\n");
			break;
		}
	}
	return 0;
}
