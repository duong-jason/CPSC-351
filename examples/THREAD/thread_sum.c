#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int sum; // this data is shared by the thread(s)
void *runner(void *param); // the thread

int main(int argc, char *argv[]) {
	pthread_t tid; // the thread identifier
	pthread_attr_t attr; // set of thread attributes

	fprintf(stderr,"process id %d \n", getpid());
	printf("\tthread id of original thread %lu\n", pthread_self());

	if (argc != 2) {
		fprintf(stderr, "usage: a.out <positive integer value>\n");
		return -1;
	}

	if (atoi(argv[1]) < 0) {
		fprintf(stderr, "%s must be >= 0\n", argv[1]);
		return -1;
	}

	// get the default attributes
	pthread_attr_init(&attr);
	// create the thread
	pthread_create(&tid, &attr, runner, argv[1]);

	fprintf(stderr,"created the thread with the id %lu \n",tid);
	// wait for the thread to exit
	pthread_join(tid, NULL);

	printf("sum = %d\n", sum);
} // end of main()

// The thread will begin control in this function
void* runner(void* param) {
	int upper = atoi(param);
	// pid_t are the same for threads of the same process
	fprintf(stderr,"process id %d \n", getpid());
	sum = (upper * (upper + 1)) / 2;
	pthread_exit(0);
}
