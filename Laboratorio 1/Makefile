all: dilation

run: dilation
	./dilation -i bike.pgm -s imagen_salida1.pgm -p imagen_salida2.pgm -N 512

run2: dilation
	./dilation -i lines.pgm -s imagen_salida1.pgm -p imagen_salida2.pgm -N 4000

dilation:
	gcc -o dilation dilation.c

clean:
	rm -f *.o dilation imagen_salida1.pgm imagen_salida2.pgm