
atree.o: atree.c atree.h
	gcc atree.c -g -Wall -ansi -pedantic -c `pkg-config --cflags --libs glib-2.0`

test: atree.o check_atree.c
	gcc check_atree.c atree.o -g -Wall -ansi -pedantic -o check_atree -L/usr/lib -I/usr/include `pkg-config --cflags --libs glib-2.0` -lcheck
    
clean:
	rm -rf *.o *.exe