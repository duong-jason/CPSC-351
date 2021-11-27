#include <sys/ipc.h> /*** REQUIRED FOR INTER-PROCESS COMMUNICATION***/
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) { // COMMAND-LINE ARGUMENTS
	/* A string buffer */
	char strBuff[1024];
	// used to store the message string from the shared_mem region
	
	/* Fill the buffer with all 0's */
	memset(strBuff, 0, 1024);
	// initializes string
	
	/* Find a segment with key 1235, of size 1024 bytes, and the region is
 	 * readable, writable, and executable (if the segment with key 1235 does 
 	 * not exit, then exit with an error
 	 */
	int segment_id = shmget(1235, 1024, IPC_EXCL | S_IRUSR | S_IWUSR); // shmget = Shared Memory Get
	// shmget(<key>, <amount of memory (bytes)>, flag[]);
	// key = use to reference to a specific region in the shared memory
	// flag[] = IPC_EXCL (executable), S_IRUSR (readable), S_IWUSR (writable)
	// NOTE: flag 'IPC_CREAT' is not needed since the shared_mem region exists
	
	/* Failed to find shared memory */
	if (segment_id < 0) {
		perror("shmget");
		exit(-1);
	}
	
	/* Get a pointer to the shared memory  */ 
	char* shared_memory = (char*) shmat(segment_id, NULL, 0); 	
	// shmat = attach to a local memory
	// return a pointer->shared_mem region
	
	/*  Error checks */
	if (!((void*)shared_memory)) {
		perror("shmat");
		exit(-1);
	}
	
	/* Copy the first 12 bytes of memory to the local buffer */
	strncpy(strBuff, shared_memory, strlen(shared_memory)+1);
	// stores a char pointer (string), shared_memory, of 12 bytes to strBuff (string)
	
	/* Print what we just read */
	fprintf(stderr, "%s\n", strBuff);
	
	/* Deallocates the memory segment */
	if (shmctl(segment_id, IPC_RMID, 0) < 0) {
		perror("shmctl");
	}
	
	return 0;
}
