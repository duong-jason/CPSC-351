#include <unistd.h> // getpagesize()
#include <sys/types.h>
#include <sys/stat.h> // defines stat structure
#include <sys/mman.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

using namespace std;

int main(int argc, char** argv) {
	if (argc < 2) { /* Make sure the command line is correct */
		cout << "<file-name> missing\n";
		exit(1);
	}

	/*
		<open()> - open the specified file
		<O_RDWR> - read-write flag
	*/
	int fd = open(argv[1], O_RDWR);

	if (fd < 0) {
		cout << "input file cannot be opened\n";
		exit(1);
	}

	struct stat stats;

	string file_size = "\nfile size: ";

	if (stat(argv[1], &stats) == 0)
		cout << "\nfile size: " << stats.st_size;
	else
		cout << "unable to get file properties\n";

	/* gets the page size */
	int page_size = getpagesize();
	cout << "\npage size is " << page_size << endl;

	/* map the file into memory */
	char* data = (char*)mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	/* did the mapping succeed ? */
	if (!data) {
		cout << "\nmapping did not succeed\n";
		exit(1);
	}

	/* print the whole file character-by-character */
	for (int fIndex = 0; fIndex < page_size; ++fIndex) {
		cout << data[fIndex];
		/*
			if ((fIndex % 1000) == 1)
			cout << endl;
		*/
	}
	cout << endl;

	/* write a string to the mapped region */
	memcpy(data, "Hello world, this is a test\n", sizeof("Hello world, this is a test"));
	/* unmap the shared memory region */
	munmap(data, page_size);
	/* close the file */
	close(fd);

	return 0;
}
