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
 * @phrase  [ contains the command line input shared by both threads ]
 * @bit     [ halts numeric thread if it starts with an alphabet or an alphabet thread starts with a numeric ]
            [ additionally determines if next word needs to be read of not ]
 * @read	[ parses the phrase into tokens (words) ]
 * @word	[ the string to be printed to the console ]
 * @alpha   [ parses out words that start with an alphabet ]
 * @numeric [ parses out numeric words that start with a numeric ]
 */
string phrase;
bool bit[2] = {0, 0};

stringstream read;
string word;

void* alpha(void*);
void* numeric(void*);

int main(int argc, char* argv[]) {
    // error checking for number of arguments
    if (argc < 2) {
        fprintf(stderr, "USAGE: %s <message string>\n", argv[0]);
        exit(-1);
    }

     // reading the phrase from terminal and storing in a global variable, phrase
    phrase = argv[1];
    // initialize parser to read first word in phrase
    read << phrase;
    read >> word;

    // creating two concurrent threads (total 3 including parent thread)
    pthread_t t1, t2;
    pthread_create(&t1, NULL, alpha, NULL);
    pthread_create(&t2, NULL, numeric, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}

void* alpha(void* arg) {
    do {
        if (isalpha(word[0]) || ispunct(word[0])) { // alphabets | symbols | punctuations 
            cout << "alpha: " << word << endl;
            bit[1] = true; // enables the next word to be read
        }
        else {
            // block t1 thread until t2 thread encounters an word starting with an alphabet
            bit[0] = true;
            // after this thread wakes, it should not read the next word as the other thread already read it 
            bit[1] = false;
            if (!read) break; // if numeric thread finishes reading the phrase, then simply stop
            while (bit[0]) continue; // if digit, then wait
        }
        if (bit[1]) { // only read next word if last word was an alphabet
            read >> word;	
        }
    } while(read);

    bit[0] = true; // frees numeric thread
    pthread_exit(0);
}

void* numeric(void* arg) {
    do {
        if (isdigit(word[0])) {
            cout << "numeric: " << word << endl;
            bit[1] = true; // enables the next word to be read
        }
        else {
             // block t2 thread until t1 thread encounters an word starting with a digit/numeric
            bit[0] = false;
            // after this thread wakes, it should not read the next word as the other thread already read it 
            bit[1] = false;
            if (!read) break; // if alpha thread finishes reading the phrase, then simply stop
            while (!bit[0]) continue; // if alphabet, then wait
        }
        if (bit[1]) { // only read next word if last word was a numeric
            read >> word;
        }
    } while(read);

    bit[0] = false; // frees alpha thread
    pthread_exit(0);
}
