/*
 * Project 2: alphanueric.cpp
 * Authors: Jason Duong (reddkingdom@csu.fullerton.edu)
 */

#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cctype>
#include <string>

using namespace std;

// global variables
string phrase;
size_t state, edge;
size_t adj_mat[3][3] = {{0, 2, 1}, {0, 1, 1}, {0, 2, 2}};

// function to be runned by each thread
void* fsm(void*);

int main(int argc, char* argv[]) {
	if (argc < 2) { // error checking for number of arguments
		fprintf(stderr, "USAGE: %s <message>\n", argv[0]);
		exit(-1);
	}

 	// reading the phrase from terminal and storing in a global variable
	// and augment the string to print the last word
	phrase = argv[1];
	phrase += " ";

	pthread_t alpha, numeric;
	// creating two threads (total 3 including parent thread)
    pthread_create(&alpha, NULL, fsm, NULL);
//    pthread_create(&numeric, NULL, fsm, NULL);

	pthread_join(alpha, NULL);
//	pthread_join(numeric, NULL);

	return 0;
}

void* fsm(void* arg) {
	state = 0;
	string buffer;

	for (char token : phrase) {
		if (isspace(token) && edge != 0) { // reset if space and prev was not space
			edge = 0;
			cout << buffer << endl;
			buffer = "";
		}
		else {
			if (isdigit(token)) { // numeric
				edge = 1;
			}
			else if (isalpha(token) || ispunct(token)) { // alphabets | symbols | punctuations
				edge = 2;
			}

			buffer += token;
		}

		state = adj_mat[state][edge];
	}

	pthread_exit(0);
}