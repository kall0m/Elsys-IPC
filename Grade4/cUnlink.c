#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include "cBuf.h"

int main() {
	int memFd = shm_open("simple_memory", O_RDONLY, 0);

	if(memFd == -1) {
		perror("Can't open file");
		return 1;
	}

	int close = shm_unlink("simple_memory");

	if(close == -1) {
		perror("Can't unlink file");
		return 2;
	}

	return 0;
}
