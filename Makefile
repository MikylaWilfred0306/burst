# Build markargv

all: burst burst.html

burst: burst.o
	gcc burst.o -larchive -lpthread -o burst

burst.html: burst.md
	ronn burst.md --manual=burst
	
view : burst.html
	man ./burst

run: burst
	./burst -o "new.txt" -i "test.txt" -n 250


clean:
	rm -f burst.o burst burst.html