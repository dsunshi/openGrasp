
rlearner.o: rlearner.c rlearner.h
	gcc rlearner.c -g -Wall -ansi -pedantic -c `pkg-config --cflags --libs glib-2.0`

test: rlearner.o check_rlearner.c
	gcc check_rlearner.c rlearner.o -g -Wall -ansi -pedantic -o check_rlearner -L/usr/lib -I/usr/include `pkg-config --cflags --libs glib-2.0` -lcheck
    
clean:
	rm -rf *.o *.exe