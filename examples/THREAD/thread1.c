#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void* thread_func(void* vargp) {
	pthread_exit((void*) 42);
}

int main() {
	int i = 29;
	pthread_t tid_original_thread = pthread_self();
	pthread_t tid_created_thread;

	printf(" thread id of original thread %lu\n", tid_original_thread);

	// arg[1] = NULL (use default features)
	// arg[2] = thread_func (when thread is created, go and run "thread_func")
	// arg[3] = NULL (paramater to function arg[2]
	pthread_create(&tid_created_thread, NULL, thread_func, NULL);

	// %lu = long int (defined in pthread_t library)
	printf(" thread id of created thread %lu\n", tid_created_thread);

	pthread_join(tid_created_thread, (void**) &i);

	printf("%d\n", i);
}
