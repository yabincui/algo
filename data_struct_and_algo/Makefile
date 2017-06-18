
srcs := $(wildcard *_test.c t1_*.c t2_*.c t3_*.c)
targets := $(patsubst %.c,%,$(srcs))
$(warning srcs = $(srcs))
$(warning targets = $(targets))

headers := $(wildcard *.h)

all: $(targets)

CFLAGS := -g
LDFLAGS := -lpthread -lrt -g

%.o : %.c Makefile $(headers)
	gcc -c -o $@ $< $(CFLAGS)

timer_test: timer_test.o timer.o utils.o
	gcc -o $@ $^ $(LDFLAGS)

priority_queue_test: priority_queue_test.o priority_queue.o utils.o
	gcc -o $@ $^ $(LDFLAGS)

trie_test: trie_test.o trie.o
	gcc -o $@ $^ $(LDFLAGS)

libutil.a: timer.o priority_queue.o utils.o trie.o
	ar -rs $@ $^

t% : t%.o libutil.a
	gcc -o $@ $@.o -L. -lutil $(LDFLAGS)

clean:
	rm -rf timer_test *.o
