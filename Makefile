FLAGS= -ansi -pedantic -Wall -g
OBJ = -ansi -pedantic -Wall -c -g 
targets = testhashtree testhashtreelist testreadtree

all: $(targets)


testhashtree: testhashtree.c hashtree.o
	gcc $(FLAGS) hashtree.o $@.c -o $@

hashtree.o: hashtree.c
	gcc $(OBJ) hashtree.c

testhashtreelist: testhashtreelist.c hashtreelist.o hashtree.o
	gcc $(FLAGS) hashtreelist.o hashtree.o $@.c -o $@

hashtreelist.o: hashtreelist.c
	gcc $(OBJ) hashtreelist.c

readtree.o: readtree.c
	gcc $(OBJ) readtree.c

testreadtree: hashtree.o hashtreelist.o readtree.o testreadtree.c
	gcc $(FLAGS) hashtree.o hashtreelist.o readtree.o testreadtree.c -o testreadtree

clean:
	rm -f *.o $(targets)

