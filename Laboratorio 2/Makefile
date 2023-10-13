all: gridding

run:
	./gridding -i hltau_test.csv -o datosgrideados -d 0.003 -N 2048 -c 10 -t 3

run2:
	./gridding -i hltau_completo_uv.csv -o datosgrideados -d 0.003 -N 2048 -c 1500 -t 15

gridding: gridding.cc
	u++ -o gridding gridding.cc

clean: # Limpia los archivos de salida y compilados
	rm -f *.o gridding