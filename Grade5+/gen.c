#include <string.h>
#include "gen.h"

//for the curious, the algorithm used is RSA
//(with hardcoded keys)

#define _gen_mod (323)
#define _gen_priv_ex (173)
#define _gen_pub_ex (5)

uint64_t ipowmod(uint64_t val, uint64_t pow, uint64_t mod)
{
	if(pow == 0)
		return 1;
	if(pow == 1)
		return val;
	int res = val;
	for(int i=0; i<(pow-1); i++)
		res = (res * val) % mod;
	return res;
}

uint16_t useKey(uint16_t val, uint16_t key, uint16_t mod)
{
	uint64_t res = ipowmod( val, key, mod );
	return res % mod;
}

uint16_t encrypt(uint8_t plain)
{
	return useKey(plain, _gen_priv_ex, _gen_mod);
}

uint8_t decrypt(uint16_t cypher)
{
	if(cypher > _gen_mod)
		abort();
	uint16_t result = useKey( cypher, _gen_pub_ex, _gen_mod );
	if(result > 255)
		abort();
    return result;
}

void generate(void* ptr, uint32_t seed)
{
	return generateFull(ptr, seed, 128);
}

int64_t verify(void* ptr)
{
	return verifyFull(ptr, 128);
}

void generateFull(void* ptr, uint32_t seed, uint32_t step)
{
	if(step == 0) abort();
	uint32_t plain = seed;
	uint8_t* plainPtr = (uint8_t*)&plain;
	uint16_t* cypherPtr = (uint16_t*)ptr;
	
	uint64_t i;
	for(i=0; i<GEN_BLOCK_SIZE / 8; i+= step)
	{
		if(step > 1 && i != 0)
			memset(cypherPtr, 0, (step - 1) * 4);

		int j;
		for(j=0; j<4; j++)
		{
			*cypherPtr = encrypt(plainPtr[j]);
			cypherPtr++;
		}
		cypherPtr += (step - 1) * 4;
		plain += step;
	}
}

int64_t verifyFull(void* ptr, uint32_t step)
{
	if(step == 0) abort();
	uint32_t plain;
	uint8_t* plainPtr = (uint8_t*)&plain;
	uint16_t* cypherPtr = (uint16_t*)ptr;
	uint32_t result;
	uint32_t expected;
	
	uint64_t i;
	for(i=0; i<GEN_BLOCK_SIZE / 8; i += step)
	{
		int j;
		for(j=0; j<4; j++)
		{
			plainPtr[j] = decrypt(*cypherPtr);
			cypherPtr++;
		}
		cypherPtr += (step - 1) * 4;
		if(i == 0)
			result = expected = plain;
		else if(plain != expected)
			return -1;
		expected += step;
	}
	return result;
}
