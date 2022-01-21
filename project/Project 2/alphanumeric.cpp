/*
 * Project 2: alphanueric.cpp
 * Authors: Jason Duong (reddkingdom@csu.fullerton.edu)
 */

#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <cctype> // isalpha | isdigit

using namespace std;

/*
 * @state   [ halts numeric thread if it starts with an alphabet or an alphabet thread starts with a numeric ]
 * @token   [ the current string to be printed to the console ]
 * @alpha   [ parses out words that start with an alphabet ]
 * @numeric [ parses out numeric words that start with a numeric ]
 */
enum State { A, N } state;
char* token;

#define wait(S)                  \
	state = S;                   \
	while (state == S) continue; \
	if (!token) break;

#define signal(S) \
	printf("%s %s\n", (S == A ? "alpha:" : "numeric:"), token);
	
void* alpha(void*);
void* numeric(void*);

int main(int argc, char* argv[]) {
    if (argc != 2) { // error checking for number of arguments
        fprintf(stderr, "USAGE: %s <message string>\n", argv[0]);
        return -1;
    }

    token = strtok(argv[1], " "); // initilize parser to read the first word in the phrase
    pthread_t t1, t2; // creating two concurrent threads (total 3 including parent thread)

    pthread_create(&t1, NULL, alpha, NULL);
    pthread_create(&t2, NULL, numeric, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}

void* alpha(void* arg) {
	do {
        if (isdigit(token[0])) { wait(N); } // only allow alpha-words to pass
		signal(A);
    } while (token = strtok(NULL, " "));

    state = N; // frees numeric thread
    pthread_exit(0);
}

void* numeric(void* arg) {
	do {
		if (!isdigit(token[0])) { wait(A); } // only allow numeric-words to pass
		signal(N);
    } while (token = strtok(NULL, " "));
    
    state = A; // frees alpha thread
    pthread_exit(0);
}
