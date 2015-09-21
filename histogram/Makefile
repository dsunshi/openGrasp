
histogram.o: histogram.c histogram.h
	gcc histogram.c -g -Wall -ansi -pedantic -c `pkg-config --cflags --libs glib-2.0`

test: histogram.o check_histogram.c
	gcc check_histogram.c histogram.o -g -Wall -ansi -pedantic -o check_histogram -L/usr/lib -I/usr/include `pkg-config --cflags --libs glib-2.0` -lcheck
    
clean:
	rm -rf *.o *.exe
