# Makefile for compiling pso module
# @author Marek Sedláček
# Brno University of Technology

COMPILER=gcc
FLAGS=-Wall -pedantic -std=c11
OUTPUT=pso
SOURCES=main.c pso.c
LIBS=-lm
EXT=.out

build:
	$(COMPILER) $(FLAGS) -o $(OUTPUT)$(EXT) $(SOURCES) $(LIBS)

O0:
	$(COMPILER) $(FLAGS) -O0 -o $(OUTPUT)_O0$(EXT) $(SOURCES) $(LIBS)
	
O1:
	$(COMPILER) $(FLAGS) -O1 -o $(OUTPUT)_O1$(EXT) $(SOURCES) $(LIBS)
	
O2:
	$(COMPILER) $(FLAGS) -O2 -o $(OUTPUT)_O2$(EXT) $(SOURCES) $(LIBS)
	
O3:
	$(COMPILER) $(FLAGS) -O3 -o $(OUTPUT)_O3$(EXT) $(SOURCES) $(LIBS)

nosse2:
	$(COMPILER) $(FLAGS) -O0 -mno-sse2 -o $(OUTPUT)_nosse2$(EXT) $(SOURCES) $(LIBS)

all: build O0 O1 O2 O3 nosse2

clean:
	rm $(OUTPUT)$(EXT)
	rm $(OUTPUT)_O0$(EXT)
	rm $(OUTPUT)_O1$(EXT)
	rm $(OUTPUT)_O2$(EXT)
	rm $(OUTPUT)_O3$(EXT)
	rm $(OUTPUT)_nosse2$(EXT)
