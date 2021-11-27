#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char** argv) {
	int a = 0;
	pid_t child_pid;

	child_pid = fork();

	a++;
	// 1) Parent' a = 1
	// 2) Child's a = 1
	if (child_pid == 0) { // only a child process enters
		pid_t grand_child_pid;
		grand_child_pid = fork();
		// grand_child = copyPCB(child)
		//		o- grand_child.a = 1
		printf("Here ?\n");
		a++;
		// Child's a = 2;
		// Grand_Child's a = 2;
		printf("a Here ? %d\n",a);
	} else {
		a--; // Parent's a = 0
	}

	printf("Hello!\n");
	fprintf(stderr, "This is process %d \n", getpid());
	printf("a is %d\n", a);

	pid_t test;
	if ((test = wait(NULL)) > 0)
		printf("return process: %d\n", test);
	else
		exit(-1);

	// Parent waits for the child
	// Child waits for grand_child
	// Grand_Child does not any children to wait for (ALL PROCESSES FINISHES EXECUTION AT THIS POINT IN TIME)

	return 0;

}
