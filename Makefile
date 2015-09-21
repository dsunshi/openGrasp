
all:
	cd atree; make
	cd gene; make
	cd hand; make
	cd histogram; make
	cd neuron; make
	cd rlearn; make
	cd shoe; make
	cd thorp; make
	cd truecount; make
	cd sm; make

clean:
	cd atree; make clean
	cd gene; make clean
	cd hand; make clean
	cd histogram; make clean
	cd neuron; make clean
	cd rlearn; make clean
	cd shoe; make clean
	cd thorp; make clean
	cd truecount; make clean
	cd sm; make clean