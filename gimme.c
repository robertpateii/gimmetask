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
int lastRandTask = -1;
char tasks[MAXTASKS][MAXTASKLEN];

enum action {
	// do not re-order unless you change the switch in main
	Quit = 0,
	Done,
	Next,
	Remove,
	Add,
	List,
	Retry,
	Get,
	Backup
};

void printAllTasks() {
	for (int i = 0; i < MAXTASKS; i++) {
		if (strlen(tasks[i]) > 0) {
			printf("#%d: %s",i+1, tasks[i]); // tasks have their own linebreak
		}
	}
}


void exportTasks() {
	FILE *filePtr;
	filePtr = fopen("task-backup.txt","w");

	if (filePtr == NULL) {
		printf("Could not open backup file for writing.");
		return;
	}

	for (int i = 0; i < MAXTASKS; i++) {
		if (strlen(tasks[i]) > 0 && tasks[i][0] != deleted && tasks[i][0] != completed ) {
			fprintf(filePtr, "%s", tasks[i]); // tasks have their own linebreak
		}
	}

	fclose(filePtr);
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
	while(lastRandTask == randTask || tasks[randTask][0] == deleted || tasks[randTask][0] == completed) {
		randTask = rand() % taskCount;
		attempts++;
		if (attempts > 20) {
			printf("Error: Found 20 removed/done/current tasks in a row. Are any valid?\n");
			return -1;
		}
	}
	printf("#%d: %s", randTask+1, tasks[randTask]);
	lastRandTask = randTask;
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
	// thanks https://sekrit.de/webdocs/c/beginners-guide-away-from-scanf.html
	char x[10];
	printf("(D)one, (N)ext, (R)emove, (A)dd, (L)ist, (G)et, (B)ackup, or (Q)uit?\n");
	if (fgets (x, 10, stdin) != NULL) {
		*x = tolower(*x);	
		if (*x == 'd') return Done;
		if (*x == 'n') return Next;
		if (*x == 'r') return Remove;
		if (*x == 'a') return Add;
		if (*x == 'l') return List;
		if (*x == 'g') return Get;
		if (*x == 'q') return Quit;
		if (*x == 'b') return Backup;
		// default
        printf("Invalid input. Commands are: \n(D)one with current, (N)ext task, (R)emove current, (A)dd more,\n(L)ist all tasks, (G)et one, (E)xport open tasks or (Q)uit?\n");
		return Retry;
	} else {
		printf("Unexpecetd null/eof, exiting.\n");
		return Quit;
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
		case 1: // done
			printf("Done w/ #%d: %s", currTaskPos+1, tasks[currTaskPos]);
			complete(currTaskPos);
			printf("Next is ");
			currTaskPos = getTask(length);
			exportTasks();
			break;
		case 2: // next
			currTaskPos = getTask(length);
			break;
		case 3: // remove
			printf("Deleted #%d: %s", currTaskPos+1, tasks[currTaskPos]);
			delete(currTaskPos);
			printf("Next is ");
			currTaskPos = getTask(length);
			exportTasks();
			break;
		case 4: // add
			addTasks(countTasks());
			printf("Current #%d: %s", currTaskPos+1, tasks[currTaskPos]);
			exportTasks();
			break;
		case 5: // list
			printAllTasks();
			printf("Current #%d: %s", currTaskPos+1, tasks[currTaskPos]);
			break;
		case 6: // retry
			printf("Current #%d: %s", currTaskPos+1, tasks[currTaskPos]);
			break;
		case 7: // get
			currTaskPos = getSpecificTask();
			break;
		case 8: // export / backup
			printf("Backing up remaining tasks to task-backup.txt, also done as part of add/rem/done");
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
