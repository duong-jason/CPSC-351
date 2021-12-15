#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void child() {
	fprintf(stderr, "\n \n Hi! I am the child. My pid is %d. I am immortal! since my parent left me\n", getpid());
	sleep(15);

	if (execlp("yes", "yes", NULL) < 0) {
		perror("execlp");
		exit(-1);
	}
}

int main(int argc, char** argv) {
	/* The process id of the child */
	pid_t child_pid;

	fprintf(stderr, "Hi! I am the parent. My pid is %d. \n", getpid());
	sleep(5);

	/* If the fork failed, then terminate. Otherwise, save the
	 * value returned by fork().
	 */

	if ((child_pid = fork()) < 0)	{
		perror("fork");
		exit(-1);
	}

	/* Are we a child process ? */
	if (child_pid == 0) {
		child(); /* Call the child function */
	} else { /* We are the parent */
		fprintf(stderr, "\n Hi! I am a parent.\nJust created a child with pid %d\n", child_pid);
		fprintf(stderr, "I will live for 60 seconds then leave my child to be an ORPHAN!");

		/* Wait for 60 seconds. NEVER CALL WAIT */
		sleep(60); // just allows us to see that the parent process existed and then
					  // terminated before its child process does
	}

	return 0;
}
