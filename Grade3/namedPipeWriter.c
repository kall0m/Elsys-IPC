#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
	int fd = open("namedPipe", O_WRONLY);

	if(fd < 0) {
		perror("open");

		return 1;
	}

	char* newYear = "And a happy new year!";

	int size = strlen(newYear);

	int wresult;

	for(int i = 0; i < size; i++) {
		int wresult = write(fd, newYear, 1);

		if(wresult < 0) {
			perror("write");

			return 2;
		}

		usleep(80000);
		newYear++;
	}

	if(close(fd) < 0) {
		perror("close");

		return 3;
	}

	return 0;
}
