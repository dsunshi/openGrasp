
neuron.o: neuron.c neuron.h
	gcc neuron.c -g -Wall -ansi -pedantic -c `pkg-config --cflags --libs glib-2.0`

test: neuron.o check_neuron.c
	gcc check_neuron.c neuron.o -g -Wall -ansi -pedantic -o check_neuron -L/usr/lib -I/usr/include `pkg-config --cflags --libs glib-2.0` -lcheck
    
clean:
	rm -rf *.o *.exe