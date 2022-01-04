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
 * @flag    [ halts numeric thread if it starts with an alphabet or an alphabet thread starts with a numeric ]
 * @token   [ the current string to be printed to the console ]
 * @alpha   [ parses out words that start with an alphabet ]
 * @numeric [ parses out numeric words that start with a numeric ]
 */
bool flag = 0;
char* token;

void* alpha(void*);
void* numeric(void*);

void alphalock() {
    if (!isalpha(token[0])) {
        flag = true;
        while (flag) {
            /*
             * let numeric-thread continue processing
             * while alpha-thread waits for interrupt
             */
            continue;
        }
    }
}

void numlock() {
    if (!isdigit(token[0])) {
        flag = false;
        while (!flag) {
            /*
             * let alpha-thread continue processing
             * while numeric-thread waits for interrupt
             */
            continue;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) { // error checking for number of arguments
        fprintf(stderr, "USAGE: %s <message string>\n", argv[0]);
        return -1;
    }

    // initilize parser to read the first word in the phrase
    token = strtok(argv[1], " ");

    // creating two concurrent threads (total 3 including parent thread)
    pthread_t t1, t2;
    pthread_create(&t1, NULL, alpha, NULL);
    pthread_create(&t2, NULL, numeric, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}

void* alpha(void* arg) {
    while (1) {
        alphalock(); // only allow alpha-words to pass
        if (!token) break;
        printf("alpha: %s\n", token);
        token = strtok(NULL, " ");
    }

    flag = true; // frees numeric thread
    pthread_exit(0);
}

void* numeric(void* arg) {
    while (1) {
        numlock(); // only allow numeric-words to pass
        if (!token) break;
        printf("numeric: %s\n", token);
        token = strtok(NULL, " ");
    }
    
    flag = false; // frees alpha thread
    pthread_exit(0);
}
