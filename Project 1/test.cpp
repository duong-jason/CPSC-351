#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

using namespace std;

#define READ_END 0 // The read end of the pipe
#define WRITE_END 1 // The write end of the pipe
#define MAX_FILE_NAME_LENGTH 1000 // The maximum length of the file name

int parentToChildPipe[2]; // The pipe for parent-to-child communications
int childToParentPipe[2]; // The pipe for the child-to-parent communication

/* error function */
void error(const char* code) {
	perror(code);
	exit(-1);
}

void computeHash() {
	char fileNameRecv[MAX_FILE_NAME_LENGTH]; // The received file name string
	memset(fileNameRecv, (char) NULL, MAX_FILE_NAME_LENGTH); // Fill the buffer with 0's

	if (close(parentToChildPipe[WRITE_END]) < 0) { error("close"); }
    if (close(childToParentPipe[READ_END]) < 0) { error("close"); }
	if (close(childToParentPipe[WRITE_END]) < 0) { error("close"); }

	if (read(parentToChildPipe[READ_END], fileNameRecv, MAX_FILE_NAME_LENGTH)) { error("read"); } // reading message from parent & then closes
    cout << fileNameRecv << endl;
	if (close(parentToChildPipe[READ_END]) < 0) { error("close"); } // reading message from parent & then closes

	// terminates a child process
	exit(0);
}

int main(int argc, char** argv) {
	if (argc < 2) { // error checking
		fprintf(stderr, "USAGE: %s <file-name>\n", argv[0]);
		exit(-1);
	}
	const string fileName(argv[1]); // Save the name of the file

    if (pipe(parentToChildPipe) < 0) { error("pipe"); }
    if (pipe(childToParentPipe) < 0) { error("pipe"); }

	pid_t pid = fork();

    if (pid > 0) {
        if (close(parentToChildPipe[READ_END]) < 0) { error("close"); }
        if (close(childToParentPipe[WRITE_END]) < 0) { error("close"); } // Close the parent read & write ends of the PC & CP pipes
        cout <<argv[1] << endl;
        if (write(parentToChildPipe[WRITE_END], argv[1], strlen(argv[1])) < 0) { error("parent write"); }
        if (close(parentToChildPipe[WRITE_END]) < 0) { error("close"); }

        if (wait(NULL) < 0) { error("wait"); } // Wait for the program to terminate
    }
    else if (pid == 0) {
        computeHash();
    }
    else {
        error("fork");
    }
	
	return 0;
}