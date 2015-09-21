
shoe.o: shoe.c shoe.h
	gcc shoe.c -g -Wall -ansi -pedantic -c `pkg-config --cflags --libs glib-2.0`

test: shoe.o check_shoe.c
	gcc check_shoe.c shoe.o -g -Wall -ansi -pedantic -o check_shoe -L/usr/lib -I/usr/include `pkg-config --cflags --libs glib-2.0` -lcheck
    
clean:
	rm -rf *.o *.exe
