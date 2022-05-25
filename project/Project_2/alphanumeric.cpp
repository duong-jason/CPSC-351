/*
 * Project 2: alphanueric.cpp
 * Authors: Jason Duong (reddkingdom@csu.fullerton.edu)
 */

#include <pthread.h>
#include <string.h> // strtok()
#include <stdio.h>
#include <cctype> // isalpha(), isdigit()

/*
 * @state   [ current state based on the current token read ]
 * @token   [ the current string to be printed to the console ]
 * @wait     [ locks a thread based on the current token read ]
 * @alpha   [ parses out words that start with an alphabet ]
 * @numeric [ parses out numeric words that start with a numeric ]
 */

enum State_t { ALPHA, NUMERIC } state;
char* token;

static void wait(const State_t&);
void* alpha(void*);
void* numeric(void*);

int main(int argc, char* argv[]) {
    // error checking for number of arguments
    if (argc == 2) {
        token = strtok(argv[1], " "); // initilize parser to read the first word in the phrase
        pthread_t t1, t2; // creating two concurrent threads (total 3 including parent thread)

        pthread_create(&t1, NULL, alpha, NULL);
        pthread_create(&t2, NULL, numeric, NULL);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
    }
    else fprintf(stderr, "%s <message-string>\n", argv[0]);
}

static void wait(const State_t& curr) {
    switch (curr) {
        case ALPHA : {
            if (isdigit(token[0])) {
                state = NUMERIC;
                while (state == NUMERIC) continue;
            }
            break;
        }
        case NUMERIC : {
            if (isalpha(token[0])) {
                state = ALPHA;
                while (state == ALPHA) continue;
            }
            break;
        }
        default : break;
    }
}

void* alpha(void* arg) {
    do {
        wait(ALPHA); // only allow alpha-words to pass
        printf("alpha: %s\n", (token ? token : "EOS (end-of-string)"));
    } while ((token = strtok(NULL, " ")) > 0);

    state = NUMERIC; // frees numeric thread
    pthread_exit(NULL);
}

void* numeric(void* arg) {
    do {
        wait(NUMERIC); // only allow numeric-words to pass
        printf("numeric: %s\n", (token ? token : "EOS (end-of-string)"));
    } while ((token = strtok(NULL, " ")) > 0);
    
    state = ALPHA; // frees alpha thread
    pthread_exit(NULL);
}
