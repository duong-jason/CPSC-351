#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h> // memcpy()
#include <unistd.h> // getpagesize()
#include <sys/stat.h> // stat structure declaration

using namespace std;

void error(const char* status) {
	cout << status << endl;
	exit(1);
}

int main(int argc, char** argv) {
	if (argc != 3) error("usage: ./<binary> <input_file> <output_file>");

	struct stat stats;
	if (stat(argv[1], &stats) != 0) error("unable to get file properties\n");

	/* declare file_size and page_size */
	int file_size = stats.st_size;
	int page_size = getpagesize();

	/* open input file */
	int in_fd = open(argv[1], O_RDWR);
	if (in_fd < 0) error("input file cannot be open");

	/* creates an output file */
	int out_fd = creat(argv[2], 0700);
	if (out_fd < 0) error("output file cannot be created");

	// initializes the output file with file_size
	int init = ftruncate(out_fd, file_size);
	if (init < 0) error("truncate failed"); 

	for (int curr_size = 0; curr_size <= file_size; curr_size += page_size) { 
		/* addr = default to OS decision
		 * page_size = how many bytes to read from file
		 * PROT_READ = set access rights to readable map region
		 * PROT_WRITE = set access rights to writable map region
		 * MAP_SHARED = enables multiple processes to map to the same file in RAM
		 * __offset = skips X number of pages
		 */
		char* in_data = (char*) mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, in_fd, curr_size);
		char* out_data = (char*) mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, out_fd, curr_size);
		if (!in_data || !out_data) error("mapping did not succeed\n");

		/* file gets modified -> dirty_bit is set -> updates copy file on disk */
		memcpy(out_data, in_data, page_size);

		/* unmap the shared memory region */
		munmap(in_data, page_size);
		munmap(out_data, page_size);
	}

	/* closes the file descriptors */
	close(in_fd);
	close(out_fd);
}
