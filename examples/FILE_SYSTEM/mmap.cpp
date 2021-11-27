#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

using namespace std;

int main(int argc, char** argv) {
	/* Make sure the command line is correct */
	if (argc < 2) {
		cout << "File-Name Missing\n";
		exit(1);
	}

	/* Open the specified file */
	int fd = open(argv[1], O_RDWR);

	if (fd < 0) {
		cout << "Input file cannot be opened\n";
		exit(1);
	}

	struct stat stats;

	string file_size = "\nfile size: ";
	file_size += stats.st_size;

	cout << (stat(argv[1], &stats) == 0 ? file_size : "Unable to get fle properties\n");

	/* Get the page size */
	int page_size = getpagesize();
	cout << "\npage size is " << page_size << endl;

	/* map the file into memory */
	char* data = (char*)mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	/* Did the mapping succeed ? */
	if (!data) {
		cout << "\nmapping did not succeed\n";
		exit(1);
	}

	/* Print the whole file character-by-character */
	for (int fIndex = 0; fIndex < page_size; ++fIndex) {
		cout << data[fIndex];
		/*
			if ((fIndex % 1000) == 1)
			cout << endl;
		*/

	}
	cout << endl;
	/* write a string to the mapped region */
	/* memcpy(data, "Hello world, this is a test\n", sizeof("Hello world, this is a test")); */

	/* unmap the shared memory region */
	munmap(data, page_size);

	/* close the file */
	close(fd);

	return 0;
}
