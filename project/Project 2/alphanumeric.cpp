/*
 * Project 2: alphanueric.cpp
 * Authors: Jason Duong (reddkingdom@csu.fullerton.edu)
 */

#include <pthread.h>
#include <iostream>
#include <string>
#include <bits/stdc++.h> // stringstream
#include <cctype> // isalpha | ispunct | isdigit

using namespace std;

/*
 * @bit     [ halts numeric thread if it starts with an alphabet or an alphabet thread starts with a numeric ]
 *          [ additionally determines if next word needs to be read of not ]
 * @read    [ parses the phrase into tokens (words) ]
 * @word    [ the string to be printed to the console ]
 * @alpha   [ parses out words that start with an alphabet ]
 * @numeric [ parses out numeric words that start with a numeric ]
 */
bool bit[2] = {0, 0};

stringstream read;
string word;

void* alpha(void*);
void* numeric(void*);

void alphalock() {
    if (!isalpha(word[0])) {
        bit[0] = bit[1] = true;
        while (bit[0]) {
            /*
             * let numeric-thread continue processing
             * while alpha-thread waits for interrupt
             */
            continue;
        }
    }
}

void numlock() {
    if (!isdigit(word[0])) {
        bit[0] = bit[1] = false;
        while (!bit[0]) {
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
        exit(-1);
    }

    // reading the phrase from terminal and storing in a global variable, phrase
    read << argv[1];
    // initilize parser to read the first word in the phrase
    read >> word;

    // creating two concurrent threads (total 3 including parent thread)
    pthread_t t1, t2;
    pthread_create(&t1, NULL, alpha, NULL);
    pthread_create(&t2, NULL, numeric, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}

void* alpha(void* arg) {
    do {
        alphalock(); // only allow alpha-words to pass
        if (!bit[1] && read) cout << "alpha: " << word << endl;
    } while (read >> word);

    bit[0] = true; // frees numeric thread
    pthread_exit(0);
}

void* numeric(void* arg) {
    do {
        numlock(); // only allow numeric-words to pass
        if (bit[1] && read) cout << "numeric: " << word << endl;
    } while (read >> word);
    
    bit[0] = false; // frees alpha thread
    pthread_exit(0);
}
