# Makefile

matrix: matrix.o coo.o csr.o
	gcc -o matrix matrix.o coo.o csr.o
        
matrix.o: matrix.c
	gcc -O1 -c matrix.c

coo.o: coo.c coo.h
	gcc -O1 -c coo.c

csr.o: csr.c csr.h coo.c coo.h
	gcc -O1 -c csr.c coo.c