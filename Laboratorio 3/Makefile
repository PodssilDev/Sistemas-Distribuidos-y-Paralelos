all: gridding secuencial

run:
	./gridding -i hltau_completo_uv.csv -o datosgrideados -d 0.003 -N 2048 -c 10000 -t 100

run_secuencial:
	./secuencial -i hltau_completo_uv.csv -o datosgrideados -d 0.003 -N 2048 -c 10000

gridding: gridding.c # Solucion con matriz compartida
	gcc -o gridding gridding.c -fopenmp -lm

secuencial: secuencial.c
	gcc -o secuencial secuencial.c -lm

clean: # Limpia los archivos de salida y compilados
	rm -f *.o gridding secuencial *.raw