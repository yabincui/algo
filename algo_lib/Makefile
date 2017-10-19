
CFLAGS := -std=c++11 -I. -g

LDFLAGS := -lgtest_main -lgtest -lpthread -g

OBJS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

TEST_OBJS := $(patsubst %.cpp,%.o,$(wildcard test/*.cpp))

all: algo_test

libalgo.a: $(OBJS)
	ar crs $@ $^

algo_test: $(TEST_OBJS) libalgo.a
	g++ -o $@ $(TEST_OBJS) -L. -lalgo $(LDFLAGS)

%.o: %.cpp Makefile
	g++ -c -o $@ $< $(CFLAGS)

clean:
	find . -name '*.o' | xargs rm -f
