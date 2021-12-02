#include <iostream>
#include <fcntl.h>

using namespace std;



int main(int argc, char** argv) {
	if (argc < 2) {
		cout << "<file-name> missing\n";
		exit(1);
	}

	return 0;
}
