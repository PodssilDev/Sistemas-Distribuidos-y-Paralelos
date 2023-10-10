all: gridding

run:
	./gridding -i hltau_test.csv -o datosgrideados -d 0.0 -N 10 -c 2 -t 2

run2:
	./gridding -i hltau_completo_uv.csv -o datosgrideados -d 0.0 -N 20 -c 10000 -t 20

gridding: gridding.cc
	u++ -o gridding gridding.cc

clean: # Limpia los archivos de salida y compilados
	rm -f *.o gridding