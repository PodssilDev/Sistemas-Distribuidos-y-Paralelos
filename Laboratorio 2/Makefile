all: gridding

run:
	./gridding -i hltau_test.csv -o datosgrideados -d 0.003 -N 2048 -c 9 -t 3

run2:
	./gridding -i hltau_completo_uv.csv -o datosgrideados -d 0.003 -N 2048 -c 15758 -t 13

gridding: gridding.cc
	u++ -o gridding gridding.cc

clean: # Limpia los archivos de salida y compilados
	rm -f *.o gridding *.raw