#ifndef _DUMB_SHM_STRUCT
#define _DUMB_SHM_STRUCT

#include <stdint.h>

struct cyclic_buf
{
	volatile uint64_t pos;
	volatile int buffer[128][4096]; //512MB (jitter)
};

#endif
