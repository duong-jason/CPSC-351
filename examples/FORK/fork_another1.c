/*
 * 2 forks() -> 3 children
 *
 *	     P
 *	   /   \
 *	  C1   C3
 *	 /
 *	C2
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
	int i;
	pid_t id;
	id = fork();
	fprintf(stderr, "This is process %d \n", getpid());
	fprintf(stderr, "id %d\n", id);
	// parent = child pid
	// child = 0
	printf("hello 1 -> %d\n", getpid());

	wait(NULL);
	// 1) parent waits
	// 2) child 1 does not have anyone to wait for -> bypass wait statement 
	// 3) parent returns and creates another fork()
	id = fork();
	fprintf(stderr, "This is process %d \n", getpid());
	// 1) the first child is the parent of the second child
	// 2) parent returns and prints its pid
	// 3) parent->child[1] (C3) prints its pid
	fprintf(stderr, "id %d\n", id);
	printf("hello 2 -> %d\n", getpid());

	wait(NULL);
	// 1) child 1 waits
	// 2) child 2 does not have anyone to wait for -> bypass wait statement
	// 3) child 1 returns from fork()
	// 4) parent->child[i] does not have anyone to wait for -> bypass wait statement

	fprintf(stderr, "return Parent %d\n", getpid());
	return 0;
}
