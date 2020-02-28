rat: main.o queue/queue.o
	gcc main.o queue/queue.o -lpthread -o rat

main.o: main.c
	gcc -c main.c

queue.o: queue/queue.c queue/queue.h
	gcc -c queue/queue.c

clean:
	rm *.o queue/*.o rat
