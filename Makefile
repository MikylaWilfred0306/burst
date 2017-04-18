# Build markargv

all: makeargv_u makeargv_t

makeargv_u: makeargv_u.o makeargv.o
	gcc makeargv_u.o makeargv.o -o makeargv_u

run: makeargv_u
	./makeargv_u "Hi"

test: makeargv_t
	./makeargv_t

clean:
	rm -f makeargv.o makeargv_u.o makeargv_t.o makeargv_u makeargv_t
