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

	uint64_t pos = 0;

	printf("starting at %ld\n", pos);

	while(true) {
		if(mem->pos == pos) {
			continue;
		}

		if(pos + 128 <= mem->pos) {
			break;
		}

		int64_t current_seed = verify((void *)mem->buffer[pos]);
		if(current_seed == -1) {
			break;
		}

		pos++;
	}

	return 0;
}
