all:
	gcc cRead.c -lrt -o read.o
	gcc cWrite.c -lrt -o write.o
	gcc cUnlink.c -lrt -o unlink.o

	sudo gnome-terminal -x sh -c './write.o'

	sleep 1
	sudo ./read.o

clean:
	sudo ./unlink.o

	rm -f *.o
