
default: build test verify

build: moments welford injest derived merge

test: build
	@echo Run this twice - the first time is long on OSX
	awk '{print $$5}' < sample.ssv  | time ./moments
	awk '{print $$5}' < sample.ssv  | time ./moments
	awk '{print $$5}' < sample.ssv  | time ./welford
	bash bin/demo1
	bash bin/demo2

verify:
	which julia || echo "Need Julia installed to run this test"
	time julia mean.jl

CLEANS += moments
moments: moments.o
	$(CC) -o $@ $^

CLEANS += welford
welford: welford.o
	$(CC) -o $@ $^

CLEANS += injest
injest: injest.o
	$(CC) -o $@ $^

CLEANS += merge
merge: merge.o
	$(CC) -o $@ $^

CLEANS += derived
derived: derived.o
	$(CC) -o $@ $^

clean:
	rm -f *.o $(CLEANS) data.log
