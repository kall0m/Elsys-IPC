#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>

#include "cyclicBuf.h"
#include "gen.h"
#include "gen.c"

int main()
{
	int memFd = shm_open("data_block", O_RDONLY, 0);
	if(memFd == -1) {
		perror("Can't open file");
		return 1;
	}

	struct cyclic_buf* mem = mmap(NULL, sizeof(struct cyclic_buf), PROT_READ, MAP_SHARED, memFd, 0);
	if(mem == MAP_FAILED) {
		perror("Can't mmap");
		return -1;
	}

	uint64_t pos = (mem->pos + 2048) % 128;
	printf("starting at %ld\n", pos);

	while(true) {
		if(mem->pos == pos) { //process 2 has to find out if process 1 has jumped process 2's position (i.e. p1 has outrunned p2 with one lap of the buffer)
			sleep(1);
			continue;
		}

		int64_t current_seed = verify((void *)mem->buffer[pos]);
		if(current_seed == -1){
			printf("Data block is inconsistent!");
			break;
		}

		printf("Parsed and verified data block: %ld\n", pos);

		pos++;
		pos %= 128;
	}

	return 0;
}
