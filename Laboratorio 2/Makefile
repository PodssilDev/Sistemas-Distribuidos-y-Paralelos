all: gridding

run:
	./gridding -i hltau_completo_uv.csv -o datosgrideados -d 0.0 -N 20 -c 3 -t 5

gridding: gridding.cc
	u++ -o gridding gridding.cc

clean: # Limpia los archivos de salida y compilados
	rm -f *.o gridding