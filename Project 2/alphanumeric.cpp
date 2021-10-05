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
#include <vector>
#include <string>

using namespace std;

/*
 * @phrase  [ contains the command line input shared by both threads ]
 * @state   [ the rows of the adj_mat ]
 * @edge    [ the columns of the adj_mat ]
 * @adj_mat [ a graph whose rules are based on a fsm (finite state machine) ] 
 */
string phrase;
size_t state, edge;
size_t adj_mat[3][3] = {{0, 2, 1}, {0, 1, 1}, {0, 2, 2}};

/*
 * @fsm [ finite state machine ] 
 *     @instance [ null ]
 *	   @output   [ a list of strings that are identified either alpha or numeric ] 
 */
void* fsm(void*);
void printtoken(const string&, const bool&);

int main(int argc, char* argv[]) {
	if (argc < 2) { // error checking for number of arguments
		fprintf(stderr, "USAGE: %s <message>\n", argv[0]);
		exit(-1);
	}

 	// reading the phrase from terminal and storing in a global variable
	phrase = argv[1];

	pthread_t alpha, numeric;
    pthread_create(&alpha, NULL, fsm, NULL); // creating two threads (total 3 including parent thread)
//    pthread_create(&numeric, NULL, fsm, NULL);

	pthread_join(alpha, NULL);
//	pthread_join(numeric, NULL);

	return 0;
}

/*
 * @fsm [ finite state machine ] 
 *     @instance [ null ]
 */

void* fsm(void* arg) {
	string token;
	bool type;
	state = 0;

	for (char word : phrase) {
		if (isspace(word) && edge != 0) { // reset if space and prev was not space
			printtoken(token, type);
			edge = 0;
			token = "";
		}
		else {
			if (isdigit(word)) { // numeric
				edge = 1;
				if (token.length() < 1) { type = 0; }
			}
			else if (isalpha(word) || ispunct(word)) { // alphabets | symbols | punctuations
				edge = 2;
				if (token.length() < 1) { type = 1; }
			}

			token += word;
		}

		state = adj_mat[state][edge];
	}
	pthread_exit(0);
}

void printtoken(const string& token, const bool& type) {
	cout << (type == 0 ? "numeric: " + token : "alpha: " + token) << endl;
}
