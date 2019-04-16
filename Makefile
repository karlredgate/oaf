
default: build test

build: moments welford

test: build
	@echo Run this twice - the first time is long on OSX
	awk '{print $$5}' < sample.ssv  | time ./moments
	awk '{print $$5}' < sample.ssv  | time ./moments
	awk '{print $$5}' < sample.ssv  | time ./welford
	time julia mean.jl

CLEANS += moments
moments: moments.o
	$(CC) -o $@ $^

CLEANS += welford
welford: welford.o
	$(CC) -o $@ $^

clean:
	rm -f *.o $(CLEANS)
