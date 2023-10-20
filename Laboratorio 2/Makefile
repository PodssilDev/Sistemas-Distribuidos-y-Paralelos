all: gridding gridding_matriz_propia

run:
	./gridding -i hltau_test.csv -o datosgrideados -d 0.003 -N 2048 -c 9 -t 3

run2:
	./gridding -i hltau_completo_uv.csv -o datosgrideados -d 0.003 -N 2048 -c 15758 -t 13

run4:
	./gridding -i hltau_completo_uv.csv -o datosgrideados -d 0.003 -N 2048 -c 15758 -t 1

run3:
	./gridding_matriz_propia -i hltau_completo_uv.csv -o datosgrideados -d 0.003 -N 2048 -c 15758 -t 13

run5:
	./gridding_matriz_propia -i hltau_completo_uv.csv -o datosgrideados -d 0.003 -N 2048 -c 15758 -t 1
	

gridding: gridding.cc 
	u++ -o gridding gridding.cc 

gridding_matriz_propia: gridding_matriz_propia.cc
	u++ -o gridding_matriz_propia gridding_matriz_propia.cc

clean: # Limpia los archivos de salida y compilados
	rm -f *.o gridding gridding_matriz_propia *.raw