all: farenheit mult_matrices addsub shuffle mult_complejos

farenheit: farenheit.c
	gcc -o farenheit farenheit.c

mult_matrices: mult_matrices.c
	gcc -o mult_matrices mult_matrices.c

addsub: addsub.c
	gcc -msse3 -o addsub addsub.c

shuffle: shuffle.c
	gcc -msse3 -o shuffle shuffle.c

mult_complejos: mult_complejos.c
	gcc -msse3 -o mult_complejos mult_complejos.c

clean:
	rm -f farenheit mult_matrices addsub shuffle mult_complejos