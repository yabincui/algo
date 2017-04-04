
all: timer_test priority_queue_test t1_1

CFLAGS := -g
LDFLAGS := -lpthread -lrt -g

%.o : %.c Makefile
	gcc -c -o $@ $< $(CFLAGS)


timer_test: timer_test.o timer.o utils.o
	gcc -o $@ $^ $(LDFLAGS)

priority_queue_test: priority_queue_test.o priority_queue.o utils.o
	gcc -o $@ $^ $(LDFLAGS)

t1_1: t1_1.o timer.o utils.o priority_queue.o
	gcc -o $@ $^ $(LDFLAGS)

clean:
	rm -rf timer_test *.o
