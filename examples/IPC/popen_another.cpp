#include <stdio.h>

int main() {
	FILE *fp;
	char line[130]; /* line of data from unix command*/

	fp = popen("ls -l", "r"); /* Issue the command. */
	while (fgets(line, sizeof(line), fp)) { printf("%s", line); } /* Read a line */

	pclose(fp);
}
