#include <sys/types.h>
#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

using namespace std;

#define BUF_SIZE 4096     // use a buffer size of 4096 bytes
#define OUTPUT_MODE 0700  // protection bits for output file

int main(int argc, char* argv[]) {
	int in_fd, out_fd, rd_count, wt_count;
	char buffer[BUF_SIZE];

	if (argc != 3) {
		cout << "Enter input file name, output file name: pgm will exit otherwise\n";
		exit(1);
	}

	/*** open the input file and create the output file ***/

	// open the input file
	in_fd = open(argv[1], O_RDONLY);

	if (in_fd < 0) {
		cout << "\n" << "input file cannot be opened\n";
		exit(1); // if it cannot be opened, exit
	}

	// create the output file
	out_fd = creat(argv[2], OUTPUT_MODE);

	if (out_fd < 0) {
		cout << "\noutput file cannot be created\n";
		exit(1); // if it cannot be created, exit
	}
	// copy loop

	while (true) {
		rd_count = read(in_fd, buffer, BUF_SIZE);
		if (rd_count <= 0) { // if end of file or error
			break;
		}

		/*
			cout<<"\n "<<"counting.......";
			cout<<"\n"<<buffer; 
		*/

		wt_count = write(out_fd, buffer, rd_count);
		// if error
		if (wt_count <= 0) {
			cout << "\nerror on writing...exiting\n";
			exit(1);
		}
	}

	close(in_fd);
	close(out_fd);

	// rd_count == 0 => no error on last read
	// exit(0) => successfully exit
	// exit(1) => error on last read

	exit(!(rd_count==0));
}
