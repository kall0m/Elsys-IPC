#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	int fd = open("namedPipe", O_RDONLY);

	if(fd < 0) {
	  perror("open");

	  return 1;
	}

	char buf;
	int wresult;

	while(read(fd, &buf, 1) > 0) {
		wresult = write(STDOUT_FILENO, &buf, 1);

		if(wresult < 0) {
			perror("write");

			return 2;
		}
	}

	wresult = write(STDOUT_FILENO, "\n", 1);

	if(wresult < 0) {
		perror("write");

		return 2;
	}

	if(close(fd) < 0) {
		perror("close");

		return 3;
	}

	return 0;
}
