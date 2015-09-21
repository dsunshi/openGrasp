
truecount.o: truecount.c truecount.h
	gcc truecount.c -g -Wall -ansi -pedantic -c `pkg-config --cflags --libs glib-2.0`

test: truecount.o check_truecount.c
	gcc check_truecount.c truecount.o -g -Wall -ansi -pedantic -o check_truecount -L/usr/lib -I/usr/include `pkg-config --cflags --libs glib-2.0` -lcheck
    
clean:
	rm -rf *.o *.exe
