#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include "cyclicBuf.h"
#include "gen.h"
#include "gen.c"

int main()
{
	int memFd = shm_open("data_block", O_CREAT | O_RDWR, S_IRWXU);
	if(memFd == -1) {
		perror("Can't open file");
		return 1;
	}

	int res;
	res = ftruncate(memFd, sizeof(struct cyclic_buf));
	if(res == -1) {
		perror("Can't truncate file");
		return res;
	}
	
	struct cyclic_buf* mem = mmap(NULL, sizeof(struct cyclic_buf), PROT_READ | PROT_WRITE, MAP_SHARED, memFd, 0);
	if(mem == MAP_FAILED) {
		perror("Can't mmap");
		return -1;
	}

	mem->pos %= 128;
	uint32_t seed = 0; //for function generate (see gen.h)

	while(true) {
		generate((void*)mem->buffer[mem->pos], seed);
		printf("Generate data block: %ld\n", mem->pos);
		mem->pos++;
		mem->pos %= 128;

		seed++;
		if(mem->pos % 10 == 0) { //see 10 data blocks per 1 second
			sleep(1);
		}
	}

	return 0;
}
