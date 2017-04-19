# Build markargv

all: burst

burst: burst.o
	gcc burst.o  -o burst


test: burst
	./burst

clean:
	rm -f burst.o
