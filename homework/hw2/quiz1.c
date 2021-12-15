// For this and next question, assume the following code is
// compiled and run on a modern linux machine (assume any
// irrelevant details have been omitted):
// Assume fork() never fails, how many times will the messge "Hello!\n" be displayed"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char** argv) {
    int a = 0;

	pid_t child_pid;
	child_pid = fork();

	a++;

	if (child_pid == 0) {
		pid_t grand_child_pid;
        grand_child_pid = fork();
		a++;
	}
	else {
		a++;
	}

    printf("Hello!\n");
	printf("%d: a is %d\n", getpid(), a);

	return 0;
}
