FLAGS=-Wall -pedantic -std=c11
OUTPUT=main
SOURCES=main.c pso.c
LIBS=-lm

build:
	gcc $(FLAGS) -o $(OUTPUT) $(SOURCES) $(LIBS)

# Build without optimization
O0:
	gcc $(FLASG) -O0 -o $(OUTPUT)_O0 $(SOURCES) $(LIBS)

