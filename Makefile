# Build markargv

all: burst_t burst

burst: burst.o burst_t.o
	gcc burst_t.o  -o burst_t
	gcc burst.o  -o burst


test: burst_t
	./burst_t

clean:
	rm -f burst.o burst_t.o