/*
 * Project 1: skel.cpp
 * Authors: Jason Duong (reddkingdom@csu.fullerton.edu)
 */

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
#define HSA_ARRAY_SIZE 6 // The maximum size of the array of hash programs
#define HASH_VALUE_LENGTH 1000 // The maximum length of the hash value
#define MAX_FILE_NAME_LENGTH 1000 // The maximum length of the file name

int parentToChildPipe[2]; // The pipe for parent-to-child communications
int childToParentPipe[2]; // The pipe for the child-to-parent communication

/* error function */
void error(const char* code) {
	perror(code);
	exit(-1);
}

// The array of hash algorithm names 
const string HSA[] = { "md5sum", "sha1sum", "sha224sum", "sha256sum", "sha384sum", "sha512sum" };

void computeHash(const string& HSA) {
	char hashValue[HASH_VALUE_LENGTH]; // The hash value buffer
	char fileNameRecv[MAX_FILE_NAME_LENGTH]; // The received file name string

	memset(fileNameRecv, (char) NULL, MAX_FILE_NAME_LENGTH); // Fill the buffer with 0's
	memset(hashValue, (char) NULL, HASH_VALUE_LENGTH); 		 // Reset the value buffer

	// close unused ends
	if (close(parentToChildPipe[WRITE_END]) || close(childToParentPipe[READ_END]) < 0) { error("close"); }

	// reading message from parent & then closes
	if (read(parentToChildPipe[READ_END], fileNameRecv, MAX_FILE_NAME_LENGTH)) { error("read"); }
	// reading message from parent & then closes
	if (close(parentToChildPipe[READ_END]) < 0) { error("close"); }

	// ~ md5sum <file-name> ~
	string cmdLine = HSA + " " + fileNameRecv;

	// issues secure hash algorithm
	FILE* f_ptr = popen(cmdLine.c_str(), "r");
	if (!f_ptr) { error("popen"); }

	// read the program output into hashValue
	if (fread(hashValue, sizeof(char), sizeof(char) * HASH_VALUE_LENGTH, f_ptr) < 0) { error("fread"); }
	// close the file pointer representing the program output
	if (pclose(f_ptr) < 0) { error("perror"); }

	// child writes to parent & then closes
	if (write(childToParentPipe[WRITE_END], hashValue, HASH_VALUE_LENGTH) < 0) { error("write"); }
	if (close(childToParentPipe[WRITE_END]) < 0) { error("close"); }

	exit(0);
}

int main(int argc, char** argv) {
	if (argc < 2) { /* error checking */
		fprintf(stderr, "USAGE: %s <file-name>\n", argv[0]);
		exit(-1);
	}

	const string fileName(argv[1]); // Save the name of the file
	
	// Run a program for each type of hash algorithm
	for (int idx = 0; idx < HSA_ARRAY_SIZE; ++idx) {
		// Create a parent-to-child and child-to-parent pipes
		if (pipe(parentToChildPipe) < 0 || pipe(childToParentPipe) < 0) {  error("pipe"); }
		// The process id 
		pid_t pid;

		// fork a child process and save the id
		if ((pid = fork()) < 0) {
			error("fork");
		}
		// I am a child
		else if (pid == 0) {
			computeHash(HSA[idx]); /* Compute the hash */
		}
		
		// Close the parent read & write ends of the PC & CP pipes
		if (close(parentToChildPipe[READ_END]) || close(childToParentPipe[WRITE_END]) < 0) { error("close"); }

		char hashValue[HASH_VALUE_LENGTH];
		memset(hashValue, (char) NULL, HASH_VALUE_LENGTH);

		// parent writes to child & then closes
		if (write(parentToChildPipe[WRITE_END], argv[1], strlen(argv[1])+1) < 0) { error("parent write"); }
		if (close(parentToChildPipe[WRITE_END]) < 0) { error("close"); }

		// parent reads from child & then closes
		// holds execution until child writes to pipe (transfer control)
		if (read(childToParentPipe[READ_END], hashValue, HASH_VALUE_LENGTH) < 0) { error("parent read"); }
		if (close(childToParentPipe[READ_END]) < 0) { error("close"); }

		// Print the hash value
		fprintf(stdout, "%s HASH VALUE: %s\n", HSA[idx].c_str(), hashValue);
		fflush(stdout);
	
		// Wait for the program to terminate
		if (wait(NULL) < 0) { error("wait"); }
	}
	
	return 0;
}