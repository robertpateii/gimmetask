#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// https://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)Randomization.html

#define MAXTASKS 100
// 80 characters, 2 completed/deleted marker and trailing space, 1 for end of line
#define MAXTASKLEN 83
char deleted = '-';
char completed = 'x';
int lastRandTask = -1;
char tasksOne[MAXTASKS][MAXTASKLEN];
char tasksTwo[MAXTASKS][MAXTASKLEN];
char (*tasks)[MAXTASKLEN] = tasksOne;
char (*tasksTemp)[MAXTASKLEN] = tasksTwo;

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
	Clear
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
		printf("Could not open task-backup file for writing.");
		return;
	}

	for (int i = 0; i < MAXTASKS; i++) {
		if (strlen(tasks[i]) > 0 ) {
			fprintf(filePtr, "%s", tasks[i]);
		}
	}

	fclose(filePtr);
}

// Save all the done/removed tasks to a file
// Remove them from the list
// Move the newest tasks into the oldest now empty slots
// ^^admittedly this approach is a little weird. Newer tasks will appear higher on the list after a clear, but at least older tasks retain their number and position instead of everything getting moved up.
void clearTasks() {
	FILE *fileDonePtr;
	fileDonePtr = fopen("task-done.txt","a");

	if (fileDonePtr == NULL) {
		printf("Could not open task-done file for writing.");
		return;
	}

	for (int i = 0; i < MAXTASKS; i++) {
		if (strlen(tasks[i]) > 0 && (tasks[i][0] == deleted || tasks[i][0] == completed) ) {
			fprintf(fileDonePtr, "%s", tasks[i]);
		}
	}

	fclose(fileDonePtr);

	// Tasks are added based on length, length is based on count of non-empty tasks.
	// So removed/completed tasks need to be replaced with open tasks so all tasks are one contiguous block
	// So for each deleted/completed, find the lowest incomplete and replace the deleted/completed
	// Which sounds dangerous, modifying an array while doing a nested loop through it
	// What if there are no incompletes below it? Set it to ""
	for (int i = 0; i < MAXTASKS; i++) {
		if (strlen(tasks[i]) > 0 && (tasks[i][0] == deleted || tasks[i][0] == completed) ) {
			for (int j = MAXTASKS; j > 0; j--) {
				if (strlen(tasks[j]) > 0 && tasks[j][0] != deleted && tasks[j][0] != completed) {
					// if the incomplete is below the completed, move the incomplete into the completed's slot
					if (j > i) {
						strcpy(tasksTemp[i], tasks[j]);
						strcpy(tasks[j], ""); // clear the moved task lets it get duped into the new list
						break; // stop moivg backwards to find an incomplete, return to the outer loop trying to find done stuff and remove it
					} else {
						// otherwise the incomplete is on the bottom, so clear it instead of the moved task
						strcpy(tasks[i], "");
						break;
					}
				}
			}
		} else {
			// it's either empty slot or an open task, so copy over the opens
			if (strlen(tasks[i]) > 0) {
				strcpy(tasksTemp[i], tasks[i]);
			}
		}
	}
	// now tasks (which is pointing to tasksOne) points to 100 empty strings
	// and tasksTemp (which is pointing to tasksTwo) points to the data we need
	// we need to swap these.
	char (*tasksTemp2)[MAXTASKLEN];
	tasksTemp2 = tasks; // save the empty string array's location to a temp pointer
	tasks = tasksTemp; // point the live pointer to where tempPointer is pointing, which has the data we need and whould be tasksTwo on the first run but will alternate
	tasksTemp = tasksTemp2; // point the temp pointer to where the empty string array is for the next run

}

void addTasks(int startPos) {
	

	char tempTask[MAXTASKLEN-2];
	printf("Enter tasks, 80 char max, type done to stop\n");
	int i = startPos;
	char* doneStr = "done\n";
	while (fgets (tempTask, MAXTASKLEN-2, stdin) != NULL) {
		if (strcmp(tempTask, doneStr)) { // 0 is match
			// TODO: may want to handle empty line being input and ignore it, I've done it on accident repeatedly
			printf("Got: %s", tempTask);
			strcpy(tasks[i], tempTask);
			i++;
		} else {
			break;
		}
	}

	printf("** Added the new ones. **\n");
}

// 1 means no backup file found, 0 means import success
int importTasks() {
	// Assumes only run at start when task list is empty
	FILE *importFilePtr;
	importFilePtr = fopen("task-backup.txt", "r");

	if (importFilePtr == NULL) {
		printf("No backup file.\n");
		return 1;
	}

	char tempTask[MAXTASKLEN-2];
	int i = 0;
	while (fgets (tempTask, MAXTASKLEN-2, importFilePtr) != NULL && i < MAXTASKS) {
			strcpy(tasks[i], tempTask);
			i++;
	}

	fclose(importFilePtr);

	printf("** Import Successful **\n");
	return 0;
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
	printf("(D)one, (N)ext, (R)emove, (A)dd, (L)ist, (G)et, (C)lear, or (Q)uit?\n");
	if (fgets (x, 10, stdin) != NULL) {
		*x = tolower(*x);	
		if (*x == 'd') return Done;
		if (*x == 'n') return Next;
		if (*x == 'r') return Remove;
		if (*x == 'a') return Add;
		if (*x == 'l') return List;
		if (*x == 'g') return Get;
		if (*x == 'q') return Quit;
		if (*x == 'c') return Clear;
		// default
		printf("Invalid input. Commands are: \n(D)one with current, (N)ext task, (R)emove current, (A)dd more,\n(L)ist all tasks, (G)et one, (C)lean & backup, or (Q)uit?\n");
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
	if (importTasks()) {
		addTasks(0);
        // TODO: ran into issue if this happens and the user kills the program here or enters no tasks, the exe stayed open unaccessible.
	} else {
		// do nothing, import success, if the user doesn't want to import, delete the backup file
	}
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
		case 8: // export / backup / clear / clean
			clearTasks();
			exportTasks();
			printf("Cleaned out done/removed and replaced backup. New task:\n");
			length = countTasks();
			currTaskPos = getTask(length);
			break;
		default:
			printf("Got unexpected action!\n");
			break;
		}
	}
	// No need to export here since it happens after each change
	return 0;
}
