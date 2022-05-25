#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void child(); // called by the first child process
void grandchild(); // called by the second child (grandchild) process

int main() {
	pid_t kid = fork();

	if (kid == 0) { // a child process will enter
		printf("Child Created %d\n", getpid());
		child();
	}
	else if (kid > 0) { // a parent process will enter
		printf("Parent Created %d\n", getpid());
	}
	else { // forked failed
		perror("Could Not Fork\n");
		exit(-1);
	}

	wait(NULL); // Parent & Child will wait
	printf("Process %d Terminated\n", getpid()); // outputs termination for reference
	return 0;
}

void child() {
	pid_t grand_child = fork(); // Parent Waiting | Child & GrandChild Executing 

	if (grand_child == 0) { // a child (technically grandchild at this time) will enter
		printf("GrandChild Created %d\n", getpid());
		grandchild();
	}
}

void grandchild() {
	printf("\nOpening Monzilla Firefox\n.\n.\n.\n");

	// execlp() function will overwrite the caller program (grandchild in this case) with a new one program (Firefox Application)
	// technically, the process is destroyed by the execlp() system call as the means of overwriting
	if (execlp("/usr/lib/firefox/firefox", "Firefox", "https://www.google.com", NULL) < 0) {
		perror("Could Not Open Monzilla Firefox");
		exit(-1);
	}
}
