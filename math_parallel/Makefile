objects = main.o testInterpolation.o testEquation.o 
source = info.c gauss.c interpolation.c
target = main

.PHONY: build test clean

build: libcm.so
libcm.so: $(source)
	gcc -fopenmp $(source) -lm -fPIC -shared -o libcm.so

test: $(target)
main: $(objects) libcm.so
	gcc $(objects) -o $(target) -L. -lcm -Wl,-rpath=/home/physicerr/math 
testInterpolation.o: testing.h testInterpolation.c
testEquation.o: testing.h testEquation.c

clean:
	@echo "Removing linked and compiled files......"
	rm -f *.o 

cleanall:
	@echo "Removing linked and compiled files......"
	rm -f *.o *.so $(target)
