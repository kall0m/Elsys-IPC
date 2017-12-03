all:
	gcc cyclicRead.c -lrt -o read.o
	gcc cyclicWrite.c -lrt -o write.o

	sudo gnome-terminal -x sh -c 'sleep 1; ./write.o'

	sudo ./read.o

clean:
	rm -f *.o
