#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int parentToChildPipe[2]; /* The pipe for parent-to-child communications */
int childToParentPipe[2]; /* The pipe for the child-to-parent communication */

#define READ_END 0  /* The read end of the pipe */
#define WRITE_END 1 /* The write end of the pipe */

void childFunc() { // The function called by a child
	char strToSend[] = "Chao Ba!"; /* The string to send to the parent */
	fprintf(stderr, "From Child: Ten con la 'Jason'\n");

	if (close(parentToChildPipe[WRITE_END]) < 0) {
		perror("close");
		exit(-1);
	}

	if (close(childToParentPipe[READ_END]) < 0) {
		perror("close");
		exit(-1);
	}

	fprintf(stderr, "'Jason' is sending message to 'Ba': %s\n", strToSend);

	sleep(2); /* Pause for 2 seconds */

	/* Send a string to the parent */
	if (write(childToParentPipe[WRITE_END], strToSend, sizeof(strToSend)) < 0) {
		perror("write");
		exit(-1);
	}

	if (close(childToParentPipe[WRITE_END]) < 0) {
		perror("close");
		exit(-1);
	}

	/* The string recieved from the parent */
	char strReceived[100];

	/** Now, lets read a message from the parent **/
	if (read(parentToChildPipe[READ_END], strReceived, sizeof(strReceived)) < 0) {
		perror("read");
		exit(-1);
	}

	fprintf(stderr, "\nJason reading message: %s\n\n", strReceived);

	/* Close the read end of the parent-to-child pipe */
	if (close(parentToChildPipe[READ_END]) < 0) {
		perror("close");
		exit(-1);
	}

	exit(0); /* The child terminates */
}

int main(int argc, char** argv) {
	/* good to error check also...*/
	if (pipe(parentToChildPipe) < 0) { /* Create a parent-to-child pipe */
		perror("front pipe");
		exit(-1);
	}

	if (pipe(childToParentPipe) < 0) { /* Create a child-to-parent pipe */
		perror("back pipe");
		exit(-1);
	}

	pid_t pid; /* The process id */

	if ((pid = fork()) < 0) { /* Fork a child process and save the id */
		perror("fork");
		exit(-1);
	}
	else if (pid == 0) { /* I am a child */
		childFunc();
	}

	/** I am a parent **/

	char strReceived[100]; /* The string received from the child */
	char strToSend[] = "Chao Jason!"; /* The string to send to the child */

	if (close(parentToChildPipe[READ_END]) < 0) { /* Close the read end of the parent-to-child pipe */
		perror("close");
		exit(-1);
	}

	if (close(childToParentPipe[WRITE_END]) < 0) { /* Close the write end of the child-to-parent pipe */
		perror("close");
		exit(-1);
	}

	/* Read the string sent by the child */
	if (read(childToParentPipe[READ_END], strReceived, sizeof(strReceived)) < 0) {
		perror("read");
		exit(-1);
	}

	fprintf(stderr, "\nBa reading message: %s\n", strReceived);
	fprintf(stderr, "'Ba' is sending a message to 'Jason': %s\n", strToSend);

	/* Send the string to the child */
	if (write(parentToChildPipe[WRITE_END], strToSend, sizeof(strToSend)) < 0) {
		perror("write");
		exit(-1);
	}

	/* Close the writing end of the parent-to-child pipe; this will cause the child's read() function to unblock. */
	if (close(parentToChildPipe[WRITE_END]) < 0) {
		perror("close");
		exit(-1);
	}

	/* Close the writing end of the parent-to-child pipe; this will cause the child's read() function to unblock. */
	if (close(childToParentPipe[READ_END]) < 0) {
		perror("close");
		exit(-1);
	}

	/* Wait for the child to exit */
	if (wait(NULL) < 0) {
		perror("wait");
		exit(-1);
	}

	return 0;
}
