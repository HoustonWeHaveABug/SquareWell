SQUARE_WELL_C_FLAGS=-O2 -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wlong-long -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wreturn-type -Wshadow -Wstrict-prototypes -Wswitch -Wwrite-strings

square_well: square_well.o
	gcc -o square_well square_well.o

square_well.o: square_well.c square_well.make
	gcc -c ${SQUARE_WELL_C_FLAGS} -o square_well.o square_well.c

clean:
	rm -f square_well square_well.o
