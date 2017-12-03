#include <stdio.h>
#include "gen.h"
#include "gen.c"

int main()
{
	char buf[GEN_BLOCK_SIZE];
	int64_t i;
	for(i=0; i <= 0xffffffff/*max 32bit*/; i++)
	{
		generate(buf, i);
		if(verify(buf) != i)
		{
			printf("Failed at %ld\n", i);
			return 1;
		}
		if(i%1000 == 0)
			printf("passed %lu\n", i);
	}
	return 0;
}
