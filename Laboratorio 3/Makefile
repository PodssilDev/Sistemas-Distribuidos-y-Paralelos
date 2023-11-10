all: gridding

run:
	./gridding -i hltau_completo_uv.csv -o datosgrideados -d 0.003 -N 2048 -c 10000 -t 5

gridding: gridding.c # Solucion con matriz compartida
	gcc -o gridding gridding.c -fopenmp -lm

clean: # Limpia los archivos de salida y compilados
	rm -f *.o gridding *.raw