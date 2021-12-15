#include <iostream>
#include <sys/types.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h> // read() & write()

using namespace std;

#define BUF_SIZE 4096     // use a buffer size of 4096 bytes
#define OUTPUT_MODE 0700  // protection bits for output file

int main(int argc, char* argv[]) {
	/* file descriptors */
    int in_fd, out_fd, rd_count, wt_count;
    char buffer[BUF_SIZE];

    if (argc != 3) {
        cout << "usage: ./<binary> <input_file> <output_file>: \n";
        exit(1);
    }

	/* open the input file
	   <in_fd> = a file descriptor (an integer value)
               = places read-pointer to the beginning of the file
	   <O_RDONLY> = read-only flag
    */
    in_fd = open(argv[1], O_RDONLY);

    if (in_fd < 0) {
        cout << "\n" << "input file cannot be opened\n";
        exit(1); // if it cannot be opened, exit
    }

	/* create the output file
        <out_fd> = opens up a empty file & places write-pointer there (an integer value)
	    <OUTPUT_MODE> = writable-flag
    */
    out_fd = creat(argv[2], OUTPUT_MODE);

    if (out_fd < 0) {
        cout << "\noutput file cannot be created\n";
        exit(1); // if it cannot be created, exit
    }

    while (true) { // copy loop
        rd_count = read(in_fd, buffer, BUF_SIZE); // reading BUF_SIZE amount of blocks at a time from in_fd into buffer & points to the new location
        if (rd_count <= 0) break; // reaches the end of the file (EOF) or error

        /*
			cout << "\n " << "counting.......";
	        cout << "\n" << buffer; 
		*/

        wt_count = write(out_fd, buffer, rd_count); // from buffer, writes to out_fd
        if (wt_count <= 0) { // if error
            cout << "\nerror on writing...exiting\n";
            exit(1);
        }
    }

    /* closes file descriptor */
    close(in_fd);
    close(out_fd);

	/* rd_count == 0: no error on last read
	   exit(0): successfully exit
	   exit(1): error on last read */

	exit(rd_count);
}
