#include <sys/types.h>
#include <sys/stat.h>

int main()
{
	mkfifo("namedPipe", 0666);
}
