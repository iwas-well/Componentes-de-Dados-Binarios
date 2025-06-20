all: componentes

componentes:
	gcc src/data.h src/data.c src/queue.h src/queue.c src/busca_componente.c -o componentes

clean:
	-rm -f componentes

