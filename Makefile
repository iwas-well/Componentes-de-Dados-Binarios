CFLAGS = -Wall
SRC = src/data.c src/queue.c src/busca_componente.c
OBJ = $(SRC:.c=.o)

componentes: $(OBJ)
	gcc $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o componentes
