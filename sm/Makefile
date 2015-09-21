
sm: sm.c sm.h
	gcc ../shoe/shoe.o ../histogram/histogram.o ../truecount/truecount.o ../hand/hand.o ../thorp/thorp.o sm.c -g -Wall -ansi -pedantic -o sm `pkg-config --cflags --libs glib-2.0`
    
clean:
	rm -rf *.o *.exe