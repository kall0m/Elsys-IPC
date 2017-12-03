#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main() {
	mkfifo("namedPipe", 0666);

	pid_t cpid;

	char buf;

	cpid = fork();

	if(cpid == -1) {
		perror("fork");

		return 1;
	} else if(cpid == 0) {
		int fd = open("namedPipe", O_RDONLY);

		if(fd < 0) {
		  perror("open");

		  return 2;
		}

		while(read(fd, &buf, 1)) {
			if(write(STDOUT_FILENO, &buf, 1) < 0) {
				perror("write");

				return 2;
			}
		}

		if(write(STDOUT_FILENO, "\n", 1) < 0) {
			perror("write");

			return 2;
		}

		if(close(fd) < 0) {
			perror("close");

			return 3;
		}

		return 0;
	} else {
		int fd = open("namedPipe", O_WRONLY);

		if(fd < 0) {
		  perror("open");

		  return 2;
		}

		char* numbers = "Merry Christmas!";

		int size = strlen(numbers);

		for(int i = 0; i < size; i++) {
			if(write(fd, numbers, 1) < 0) {
				perror("write");

				return 2;
			}

			usleep(80000);
			numbers++;
		}

		if(close(fd) < 0) {
			perror("close");

			return 3;
		}

		wait(0);

		return 0;
	}
}
