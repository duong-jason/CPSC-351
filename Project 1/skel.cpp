#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

using namespace std;

int parentToChildPipe[2]; /* The pipe for parent-to-child communications */
int childToParentPipe[2]; /* The pipe for the child-to-parent communication */

#define READ_END 0 /* The read end of the pipe */
#define WRITE_END 1 /* The write end of the pipe */
#define HASH_PROG_ARRAY_SIZE 6 /* The maximum size of the array of hash programs */
#define HASH_VALUE_LENGTH 1000 /* The maximum length of the hash value */
#define MAX_FILE_NAME_LENGTH 1000 /* The maximum length of the file name */

/* The array of hash algorithm names */
const string HSA[] = { "md5sum", "sha1sum", "sha224sum", "sha256sum", "sha384sum", "sha512sum" };

/**
 * The function called by a child
 * @param HSA - the name of the hash program
 */
void computeHash(const string& HSA) {
	char hashValue[HASH_VALUE_LENGTH]; /* The hash value buffer */
	char fileNameRecv[MAX_FILE_NAME_LENGTH]; /* The received file name string */

	memset(fileNameRecv, (char)NULL, MAX_FILE_NAME_LENGTH);	 /* Fill the buffer with 0's */
	
	/** TODO: Now, lets read a message from the parent **/
	
	/* Glue together a command line <PROGRAM NAME>.
 	 * For example, sha512sum fileName.
 	 */
	string cmdLine(HSA);
	cmdLine += " ";
	cmdLine += fileNameRecv;
	
    /* TODO: Open the pipe to the program (specified in cmdLine)
	* using popen() and save the ouput into hashValue. See popen.cpp
    * for examples using popen.
	.
	.
	.
	*/
		
	memset(hashValue, (char)NULL, HASH_VALUE_LENGTH); /* Reset the value buffer */
		
	/* TODO: Send a string to the parent
 	 .
	 .
	 .
	*/

	/* The child terminates */
	exit(0);
}

int main(int argc, char** argv) {
	if (argc < 2) { /* Check for errors */
		fprintf(stderr, "USAGE: %s <FILE-NAME>\n", argv[0]);
		exit(-1);
	}
	
	string fileName(argv[1]); /* Save the name of the file */
	pid_t pid; /* The process id */
	
	/* Run a program for each type of hashing algorithm hash algorithm */
	for (int idx = 0; idx < HASH_PROG_ARRAY_SIZE; ++idx) {

		/** TODO: create two pipes **/
		
		/* Fork a child process and save the id */
		if ((pid = fork()) < 0) {
			perror("fork");
			exit(-1);
		} else if (pid == 0) { /* I am a child */
			/** TODO: close the unused ends of two pipes **/

			computeHash(HSA[idx]); /* Compute the hash */
		}
		
		/* I am the parent */

		/** TODO: close the unused ends of two pipes. **/

		/* The buffer to hold the string received from the child */	
		char hashValue[HASH_VALUE_LENGTH];
		
		/* Reset the hash buffer */
		memset(hashValue, (char)NULL, HASH_VALUE_LENGTH);
		
			
		/* TODO: Send the string to the child 
		 .
		 .
		 .
		 */
		
		/* TODO: Read the string sent by the child 
		 .
		 .	
		 .
		 */
		
		/* Print the hash value */
		fprintf(stdout, "%s HASH VALUE: %s\n", HSA[idx].c_str(), hashValue);
		fflush(stdout);
	
		if (wait(NULL) < 0) { /* Wait for the program to terminate */	
			perror("wait");
			exit(-1);
		}
	}
	
	return 0;
}
