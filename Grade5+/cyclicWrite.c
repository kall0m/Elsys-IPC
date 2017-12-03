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
	int memFd = shm_open("simple_memory", O_CREAT | O_RDWR, S_IRWXO);
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

	/*char buf[GEN_BLOCK_SIZE];
	int64_t i;
	for(i=0; i <= 0xffffffff/*max 32bit; i++)
	{
		generate((void*)mem->array, i);
		if(verify(buf) != i)
		{
			printf("Failed at %ld\n", i);
			return 1;
		}
		if(i%1000 == 0)
			printf("passed %lu\n", i);
	}*/

	mem->pos %= 4096;

	int currVal = mem->array[mem->pos];

	while(true) {
		mem->pos++;
		mem->pos %= 4096;

		mem->array[mem->pos] = currVal;
		generate((void*)mem->array, mem->pos);
		printf("==%ld==\n", mem->pos);

		currVal++;

		if(mem->pos % 512 == 0) {
			sleep(1);
		}
	}

	return 0;
}
