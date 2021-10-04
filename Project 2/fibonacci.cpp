/*
 * Project 2: fibonacci.cpp
 * Authors: Jason Duong (reddkingdom@csu.fullerton.edu)
 */

#include <pthread.h>
#include <iostream>
#include <vector>
#include <stdlib.h> // atoi
#include <sys/types.h>
#include <unistd.h>
#include <string>

using namespace std;

vector<size_t> f; // global data structure
void* fibonacci(void*); // thread caller function

int main(int argc, char* argv[]) {
	if (argc < 2) { // error checking
		fprintf(stderr, "USAGE: %s <nonnegative integer>\n", argv[0]);
		exit(-1);
	}

   if (atoi(argv[1]) < 0) {
    	fprintf(stderr, "instance must be >= 0 (nonnegative integer)\n");
    	return -1;
   }

	pthread_t tid; // declare thread id
	pthread_create(&tid, NULL, fibonacci, argv[1]); // creates child thread;
	pthread_join(tid, NULL); // parent thread waits for child thread to terminate

	for (size_t elt : f) { cout << elt << ", "; }
	cout << endl;

	return 0;
}

void* fibonacci(void* arg) {
	int n = atoi((char*)arg);

	/* base cases | initial values */
	f.push_back(0);
	f.push_back(1);

	// for each iteration, we insert a new element based on the Fibonacci principle
	for (size_t i = 0; i < n-1; ++i) {
		f.push_back(f[i] + f[i+1]);
	}

	pthread_exit(0);
}