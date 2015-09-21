
hand.o: hand.c hand.h
	gcc hand.c -g -Wall -ansi -pedantic -c `pkg-config --cflags --libs glib-2.0`

test: hand.o check_hand.c
	gcc check_hand.c hand.o -g -Wall -ansi -pedantic -o check_hand -L/usr/lib -I/usr/include `pkg-config --cflags --libs glib-2.0` -lcheck
    
clean:
	rm -rf *.o *.exe