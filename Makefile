# Build markargv

all: burst_t burst

burst: burst.o
	chmod +x burst_t
	gcc burst.o  -o burst


test: burst_t
	./burst_t

clean:
	rm -f burst.o
