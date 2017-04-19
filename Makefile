# Build markargv

all: burst

burst: burst.o
	gcc burst.o  -o burst


test: burst
	./burst

run: burst
	./burst test.txt new.txt


clean:
	rm -f burst.o
