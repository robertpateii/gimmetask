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

void addTasks() {
	
	initTasks();

	// during testing ensure we have some tests tasks w/o having to enter them
	tasks[0] = "test task 0\n";
	tasks[1] = "test task 1\n";

	char tempTask[81];
	printf("Enter a tasks, 80 char max, ctrl-d to stop\n");
	int i = 2;
	while (fgets (tempTask, 81, stdin) != NULL) {
		printf("Got %s", tempTask);
		// all new tasks are getting saved as the last, it's acting like javascript, what gives?
		// so i is incrementing, and tempTask is getting updated but it's running at the end
		// when tempTask is the last input task instead of after each task.
		// but the above printf is showing each new task
		// it really does seem like some sort of optimization or closure issue
		// is it because when i init the array i'm doing it to "" and it's just the same pointer all the way instead
		// of separate pointers to the start of separate strings?
		// i need to go study up on arrays, accessing them, and char arrays
		tasks[i] = tempTask;
		printf("stored %s", tasks[i]); // <- fine here
		i++;
	}
	printf("test-task2: %s|", tasks[2]); // <- wrong here, it's def just using one shared location for every array entry
	printf("test-task3: %s|", tasks[3]); // <- wrong here

	// also need to clear the ctrl-d lest the next line read pick it up
	// so many ways to do this, trying https://stackoverflow.com/a/45470329
	// this did nothing
	/*
	char buf[5];
	if (fgets(buf, sizeof buf, stdin) == NULL) {
		// Handle EOF or Error
		// do nothing?
	}
	*/

	printf("done taking tasks, now printing all:\n");

	for (int i = 0; i < 100; i++) {
		if (strlen(tasks[i]) > 0) {
			printf("task-%d: %s|",i, tasks[i]); // tasks have their own line break, right?
		}

	}

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
		printf("got: %s\n",name);
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
				printf("No match, needed C, S, D, or E. Got: %s\n",name);
				error = 1;
				break;
		}
	} else {
		printf("No match, needed C, S, D, or E. Got: %s\n",name);
		error = 1;
	}
	return error;
}

int main() {
	srand(time(0));
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
