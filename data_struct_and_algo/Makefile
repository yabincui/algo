
all: timer_test priority_queue_test t1_1 trie_test t1_2 t1_3 t1_4

CFLAGS := -g
LDFLAGS := -lpthread -lrt -g

%.o : %.c Makefile
	gcc -c -o $@ $< $(CFLAGS)


timer_test: timer_test.o timer.o utils.o
	gcc -o $@ $^ $(LDFLAGS)

priority_queue_test: priority_queue_test.o priority_queue.o utils.o
	gcc -o $@ $^ $(LDFLAGS)

trie_test: trie_test.o trie.o
	gcc -o $@ $^ $(LDFLAGS)

t1_1: t1_1.o timer.o utils.o priority_queue.o
	gcc -o $@ $^ $(LDFLAGS)

t1_2: t1_2.o trie.o
	gcc -o $@ $^ $(LDFLAGS)

t1_3: t1_3.o
	gcc -o $@ $^ $(LDFLAGS)
	
t1_4: t1_4.o
	gcc -o $@ $^ $(LDFLAGS)

clean:
	rm -rf timer_test *.o
