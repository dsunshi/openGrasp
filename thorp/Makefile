
thorp.o: thorp.c thorp.h
	gcc thorp.c -g -Wall -ansi -pedantic -c `pkg-config --cflags --libs glib-2.0`

test: thorp.o check_thorp.c
	gcc check_thorp.c ../hand/hand.o thorp.o -g -Wall -ansi -pedantic -o check_thorp -L/usr/lib -I/usr/include `pkg-config --cflags --libs glib-2.0` -lcheck
    
clean:
	rm -rf *.o *.exe