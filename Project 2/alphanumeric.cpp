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
 * @bool    [ halts numeric thread if start is an alphabet or alphabet thread if start is a numeric ]
 * @i       [ number of words iterated in total by both threads ]
 * @alpha   [ parses out words that start with an alphabet ]
 * @numeric [ parses out numeric words that start with a numeric ]
 */
string phrase;
bool flag;
int i = 0;
void* alpha(void*);
void* numeric(void*);

int main(int argc, char* argv[]) {
	// error checking for number of arguments
	if (argc < 2) {
		fprintf(stderr, "USAGE: %s <message string>\n", argv[0]);
		exit(-1);
	}

 	// reading the phrase from terminal and storing in a global variable
	phrase = argv[1];
	// initialize flag with starting char
	flag = isdigit(phrase[0]) ? true : false;

	pthread_t t1, t2;
   pthread_create(&t1, NULL, alpha, NULL); // creating two concurrent threads (total 3 including parent thread)
	pthread_create(&t2, NULL, numeric, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	return 0;
}

void* alpha(void* arg) {
	string word;
	stringstream read(phrase);
	int n = 0; // number of words read by only thread t1

	while (read >> word) {
		if (isalpha(word[0]) || ispunct(word[0])) { // alphabets | symbols | punctuations 
			cout << "alpha: " << word << endl;
			n++;
			i++;
		}
		else if (n < i && !flag) { // skip words already read by t2 thread
			n++;
		}
		else {
			flag = true; // block t1 thread until t2 thread encounters an word starting with an alphabet
			while (flag) continue; // if digit, then wait
			n++;
		}
	}

	flag = true; // frees numeric thread
	pthread_exit(0);
}

void* numeric(void* arg) {
	string word;
	stringstream read(phrase);
	int n = 0; // number of words read by only thread t2

	while (read >> word) { // parses out words from string, no need for tokenizer
		if (isdigit(word[0])) {
			cout << "numeric: " << word << endl;
			n++;
			i++;
		}
		else if (n < i && flag) { // skip word already read by t1 thread
			n++;
		}
		else {
			flag = false; // block t2 thread until t1 thread encounters an word starting with a digit/numeric 
			while (!flag) continue; // if alphabet, then wait
			n++;
		}
	}

	flag = false; // frees alpha thread
	pthread_exit(0);
}
