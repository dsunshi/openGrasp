
gene.o: gene.c gene.h atree.o
	gcc gene.c ../atree/atree.o -g -Wall -ansi -pedantic -c `pkg-config --cflags --libs glib-2.0`
	
atree.o:
	cd ../atree; make

test: gene.o check_gene.c atree.o
	gcc check_gene.c gene.o ../atree/atree.o -g -Wall -ansi -pedantic -o check_gene -L/usr/lib -I/usr/include `pkg-config --cflags --libs glib-2.0` -lcheck
    
clean:
	rm -rf *.o *.exe
