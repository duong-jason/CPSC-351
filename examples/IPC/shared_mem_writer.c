/**
 * This program illustrates the basics of shared memory.
 * This program allocates a piece of memory, and writes
 * the string that user entered at the command line into
 * the memory location.
 */

#include <sys/ipc.h> /*** REQUIRED FOR INTER-PROCESS COMMUNICATION***/
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
	argc = number of arguments (including ./<exec_file>)
	char** argv = pointer to an array of arguments
*/

int main(int argc, char** argv) { // COMMAND-LINE ARGUMENTS
	/* Check the command line parameters */
	if (argc < 2) {
		fprintf(stderr, "Usage: %s 'string to write'\n", argv[0]);
		exit(-1);
	}	
	
	/* Since we will be allocating only 1024 bytes of 
 	 * shared memory, our string cannot exceed that size
 	 */
	if (strlen(argv[1]) > 1024) {
		fprintf(stderr, "Your string is too big (should be <= 1024 characters)\n");
		exit(-1);
	}
	
	/* Allocate a segment with key 1235, of size 1024 bytes, and the region is
 	 * readable, writable, and executable (IPC_CREAT = create the segment if 
 	 * does not exist).
 	 */
	int segment_id = shmget(1235, 1024, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR); 
	// IPC_CREAT = need for creating the shared memory for the first time
	
	/* Failed to allocate shared memory */
	if (segment_id < 0) {
		perror("shmget");
		exit(-1);
	}
	
	/* Get a pointer to the shared memory  */ 
	char* shared_memory = (char*) shmat(segment_id, NULL, 0); 	
	// return a pointer->shared_mem region stored in the RAM
	
	/*  Error checks */
	if (!((void*)shared_memory)) {
		perror("shmat");
		exit(-1);
	}
	
	/* Writes a string to the shared memory */
	snprintf(shared_memory, strlen(argv[1])+1, "%s", argv[1]);
	// writes string, argv[1], with length, strlen(argv[1]+1)
	// +1 is needed for the null-terminated character, '\0'
	
	return 0;
}
