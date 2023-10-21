all: gridding gridding_matriz_propia

# Ejemplos matriz compartida
run:
	./gridding -i hltau_completo_uv.csv -o datosgrideados -d 0.003 -N 2048 -c 1000 -t 1

run2:
	./gridding -i hltau_completo_uv.csv -o datosgrideados -d 0.003 -N 2048 -c 1000 -t 5

run3:
	./gridding -i hltau_completo_uv.csv -o datosgrideados -d 0.003 -N 2048 -c 1000 -t 10

run4:
	./gridding -i hltau_completo_uv.csv -o datosgrideados -d 0.003 -N 2048 -c 1000 -t 15

# ejemplos matriz propia
run5:
	./gridding_matriz_propia -i hltau_completo_uv.csv -o datosgrideados -d 0.003 -N 2048 -c 1000 -t 1

run6:
	./gridding_matriz_propia -i hltau_completo_uv.csv -o datosgrideados -d 0.003 -N 2048 -c 1000 -t 5

run7:
	./gridding_matriz_propia -i hltau_completo_uv.csv -o datosgrideados -d 0.003 -N 2048 -c 1000 -t 10

run8:
	./gridding_matriz_propia -i hltau_completo_uv.csv -o datosgrideados -d 0.003 -N 2048 -c 1000 -t 15
	

gridding: gridding.cc # Solucion con matriz compartida
	u++ -multi -nodebug -o gridding gridding.cc 

gridding_matriz_propia: gridding_matriz_propia.cc # Solucion con matriz propia
	u++  -multi -nodebug -o gridding_matriz_propia gridding_matriz_propia.cc

clean: # Limpia los archivos de salida y compilados
	rm -f *.o gridding gridding_matriz_propia *.raw