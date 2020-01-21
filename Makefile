COMPILER=gcc
FLAGS=-Wall -pedantic -std=c11 -g
OUTPUT=main
SOURCES=main.c pso.c
LIBS=-lm

build:
	$(COMPILER) $(FLAGS) -o $(OUTPUT) $(SOURCES) $(LIBS)

O0:
	$(COMPILER) $(FLAGS) -O0 -o $(OUTPUT)_O0 $(SOURCES) $(LIBS)

nosse2:
	$(COMPILER) $(FLAGS) -O0 -mno-sse2 -o $(OUTPUT)_nosse2 $(SOURCES) $(LIBS)

all: build O0 nosse2

clean:
	rm $(OUTPUT)
	rm $(OUTPUT)_O0
	rm $(OUTPUT)_nosse2
