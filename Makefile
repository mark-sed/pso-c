FLAGS=-Wall -Werror -pedantic -std=c99
OUTPUT=main
SOURCES=main.c pso.c

build:
	gcc $(FLAGS) -o $(OUTPUT) $(SOURCES)

# Build without optimization
O0:
	gcc $(FLASG) -O0 -o $(OUTPUT)_O0 $(SOURCES)

