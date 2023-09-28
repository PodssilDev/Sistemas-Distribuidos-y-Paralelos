all: dilation

run: dilation # Test de ejemplo con la imagen bike.pgm de 512 x 512
	./dilation -i bike.pgm -s imagen_secuencial.pgm -p imagen_paralela.pgm

run2: dilation # Test de ejemplo con la imagen lines.pgm de 4000 x 4000
	./dilation -i lines.pgm -s imagen_secuencial.pgm -p imagen_paralela.pgm

funciones.o: funciones.c funciones.h
	gcc -c funciones.c -lm

dilation: dilation.c funciones.o
	gcc -o dilation dilation.c funciones.o -lm

clean: # Limpia los archivos de salida y compilados
	rm -f *.o dilation imagen_secuencial.pgm imagen_paralela.pgm