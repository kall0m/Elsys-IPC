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

	uint64_t pos = (mem->pos + 64) % 128;
	//uint64_t pos = 0;

	uint64_t oldPos = pos;

	int posTime = 0;
	int memPosTime = 0;

	printf("starting at %ld\n", pos);

	while(true) {
		if(mem->pos == pos) {
			usleep(70000);
			continue;
		}

		int64_t current_seed = verify((void *)mem->buffer[pos]);
		if(current_seed == -1) {
			break;
		}

		if(mem->pos == 127) { //count every rotation of writer
			memPosTime++;
		}

		if(pos == 127) { //count every rotation of reader
			posTime++;
		}

		if(memPosTime > posTime) { //if writer has outrunned reader, do something with reader's pos (acknowledge reader by some way - resynchronizing (start from beginning) or break)
			//pick one of them :D

			//pos = 0;
			pos = mem->pos;
			//pos = oldPos; (uncomment part 1)
			//break;
		}

		pos++;
		pos %= 128;

		//oldPos = pos; (uncomment part 2)
	}

	return 0;
}
