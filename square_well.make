SQUARE_WELL_C_FLAGS=-O2 -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wlong-long -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wreturn-type -Wshadow -Wstrict-prototypes -Wswitch -Wwrite-strings

SQUARE_WELL_OBJS=square_well.o avl_tree.o

square_well: ${SQUARE_WELL_OBJS}
	gcc -o square_well ${SQUARE_WELL_OBJS}

square_well.o: square_well.c avl_tree.h square_well.make
	gcc -c ${SQUARE_WELL_C_FLAGS} -o square_well.o square_well.c

avl_tree.o: avl_tree.c avl_tree.h square_well.make
	gcc -c ${SQUARE_WELL_C_FLAGS} -o avl_tree.o avl_tree.c

clean:
	rm -f square_well ${SQUARE_WELL_OBJS}
