/*
 * This program illustrates the usage of the pipe()
 * function in order to enable parent child communication
 */

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

#define READ_END 0  /* The read end of the pipe */
#define WRITE_END 1 /* The write end of the pipe */

int fd[2]; /* The array to contain the file descriptors the read and write ends of the pipe. */

void childFunc() { // The function to be called by the child
	#define RECV_BUFFER_SIZE 100 	   /* The size of receive message buffer */
	char recvBuffer[RECV_BUFFER_SIZE]; /* The buffer to store the message received from the parent */

	if (close(fd[WRITE_END]) < 0) { // The child closes his write end of the pipe -- he does not need it
		perror("close");
		exit(-1);
	}

	if (read(fd[READ_END], recvBuffer, RECV_BUFFER_SIZE) < 0) { /* Try to receive the message from the parent */
		perror("read");
		exit(-1);
	}

	printf("Got message from parent: %s\n", recvBuffer); /* Print the message */

	if (close(fd[READ_END]) < 0) { // The child closes his read end of the pipe -- he does not need it anymore
		perror("close");
		exit(-1);
	}

	// exit(0);
}

void parentFunc() { // The function called by the parent process
	char message[] = "Hello child!"; /* The message to send to the child over the pipe */

	if (close(fd[READ_END]) < 0) { // The parent closes his read end of the pipe -- he does not need it
		perror("close");
	}

	if (write(fd[WRITE_END], message, strlen(message)) < 0) { /* Write the string to the child over the pipe */
		perror("write");
		exit(-1);
	}

	if (close(fd[WRITE_END]) < 0) { // The parent closes his write end of the pipe -- he does not need it anymore.
		perror("close");
	}
}

int main() {
	if (pipe(fd) < 0) { /* Create the (parent) pipe */
		perror("pipe");
		exit(-1);
	}

	pid_t pid = fork(); /* Create a child process (Child process inherits the file descriptors of the pipe) */

	if (pid < 0) { /* Make sure the fork succeeded */
		perror("fork");
		exit(-1);
	}

	/* If this is a child process, then calls the child function
	 * NOTE: the pipes are files -> inherited by the child process
	 */
	if (pid == 0) {
		cout << "here 3" << endl;
		childFunc();
		cout << "here 4" << endl;
	}

	/* If this is the parent process, then call the parent function */
	else {
		cout << "here 1" << endl;
		parentFunc();
		cout << "here 2" << endl;

		if (wait(NULL) < 0) { /* Wait for the child process to terminate */
			perror("wait");
			exit(-1);
		}

		cout << "here 5" << endl;
		exit(0);
	}

	return 0;
}
