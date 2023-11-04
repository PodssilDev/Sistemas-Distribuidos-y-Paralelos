all: HelloWorldOMP PEP1_2021OMP menor reduction simple integral maxArrayOMP promedioOMP

HelloWorldOMP: HelloWorldOMP.c
	gcc -o HelloWorldOMP HelloWorldOMP.c -fopenmp

PEP1_2021OMP: PEP1_2021OMP.c
	gcc -o PEP1_2021OMP PEP1_2021OMP.c -fopenmp

menor: menor.c
	gcc -o menor menor.c -fopenmp

reduction: reduction.c
	gcc -o reduction reduction.c -fopenmp

simple: simple.c
	gcc -o simple simple.c -fopenmp

integral: integral.c
	gcc -o integral integral.c -fopenmp

maxArrayOMP: maxArrayOMP.c
	gcc -o maxArrayOMP maxArrayOMP.c -fopenmp

promedioOMP: promedioOMP.c
	gcc -o promedioOMP promedioOMP.c -fopenmp

clean:
	rm -f HelloWorldOMP PEP1_2021OMP menor reduction simple integral maxArrayOMP promedioOMP