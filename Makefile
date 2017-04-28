# Build markargv

all: burst

burst: burst.o
	gcc burst.o -lpthread -o burst


run: burst
	./burst -o "new.txt" -i "test.txt"


clean:
	rm -f burst.o
