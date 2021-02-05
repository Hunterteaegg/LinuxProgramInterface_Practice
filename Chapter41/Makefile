CC=gcc
FLAGS=-static

41-1-dynamic.o:41-1.c
	$(CC) $^ -o $@
41-1-static.o:41-1.c
	$(CC) $^ $(FLAGS) -o $@

.PHONY:clean
clean:
	rm *.o
