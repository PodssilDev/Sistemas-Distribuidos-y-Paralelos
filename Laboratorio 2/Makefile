all: gridding gridding_m

run:
	./gridding -i hltau_test.csv -o datosgrideados -d 0.003 -N 2048 -c 9 -t 3

run2:
	./gridding -i hltau_completo_uv.csv -o datosgrideados -d 0.003 -N 2048 -c 15758 -t 13

run3:
	./gridding_m -i hltau_completo_uv.csv -o datosgrideados -d 0.003 -N 2048 -c 15758 -t 13

gridding: gridding.cc
	u++ -o gridding gridding.cc

gridding_m: gridding_matriz.cc
	u++ -o gridding_m gridding_matriz.cc

clean: # Limpia los archivos de salida y compilados
	rm -f *.o gridding gridding_m *.raw