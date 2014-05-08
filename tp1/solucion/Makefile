EXTRAFLAGS:= -Wall -Werror -pedantic --std=c99 -ggdb -O0
EXTRACPPFLAGS:= -Wall -Werror -pedantic -ggdb -O0 -Wno-long-long -Wno-variadic-macros -I.
NASMEXTRAFLAGS:=-f elf64 -g -F dwarf 
OBJS=lista_colgante.o
COBJS=clista_colgante.o
HEADS=test-o-matic.h lista_colgante.h base.h
INCLUDE=-include /usr/include/errno.h

.PHONY: all clean

tester: $(OBJS) $(COBJS) $(HEADS) test-o-matic.o
	 g++ $(INCLUDE) -o tester $(EXTRACPPFLAGS) tester.cpp $(OBJS) $(COBJS) test-o-matic.o

testerc: $(COBJS) $(HEADS) test-o-matic.o
	 g++ $(INCLUDE) -o tester $(EXTRACPPFLAGS) tester.cpp $(COBJS) test-o-matic.o

main: main.c $(OBJS) $(COBJS) $(HEADS)
	g++ $(INCLUDE) -o $@ $(EXTRACPPFLAGS) $< $(OBJS) $(COBJS)

mainc: main.c $(COBJS) $(HEADS)
	g++ $(INCLUDE) -o $@ $(EXTRACPPFLAGS) $< $(COBJS)

generador: test-o-matic.o
	g++ $(INCLUDE) -o generador_test $(EXTRACPPFLAGS) generador_tests.cpp test-o-matic.o $(COBJS)


%.o: %.asm
	nasm $(NASMEXTRAFLAGS) -o $@ $<

%.o: %.c
	gcc -c $(EXTRAFLAGS) -o $@ $<

%.o: %.cpp
	g++ -c $(EXTRACPPFLAGS) -o $@ $<

bundle:
	mkdir -p tp1bundle
	mkdir -p tp1bundle/solucion
	mkdir -p tp1bundle/tests
	cp ../enunciado/enunciado-tp1.pdf versiones.txt tp1bundle
	cp ../tests/*.in tp1bundle/tests
	cp ../tests/*.catedra.out tp1bundle/tests
	cp lista_colgante.asm lista_colgante.h base.h tests.sh testuno.sh pruebacorta.sh main.c tester.cpp test-o-matic.cpp test-o-matic.h generador_tests.cpp gdb_stl_views.gdb Makefile tests-simples.in tp1bundle/solucion
	tar -czf tp1.tar.gz tp1bundle
	#rm -rf tp1bundle

all: $(EXE) tester

clean:
	rm -f $(OBJS) $(COBJS) tester main solc
	rm -f $(EXE) *.o

