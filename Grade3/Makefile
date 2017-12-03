all:
	gcc namedPipeMaker.c -o make.o
	gcc namedPipeReader.c -o read.o
	gcc namedPipeWriter.c -o write.o

	./make.o
	
	gnome-terminal -x sh -c './read.o; sleep 0.5'

	./write.o

process:
	gcc namedPipeProcess.c -o process.o

	gnome-terminal -x sh -c './process.o; sleep 0.5'

clean:
	rm -f *.o namedPipe a.out
