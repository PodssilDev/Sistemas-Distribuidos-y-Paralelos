all: HelloWorldOMP PEP1_2021OMP

HelloWorldOMP: HelloWorldOMP.c
	gcc -o HelloWorldOMP HelloWorldOMP.c -fopenmp

PEP1_2021OMP: PEP1_2021OMP.c
	gcc -o PEP1_2021OMP PEP1_2021OMP.c -fopenmp

clean:
	rm -f HelloWorldOMP PEP1_2021OMP