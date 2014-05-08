#/bin/bash
VALGRIND=valgrind
TESTDIR=../tests
BINFILE=./tester
EXT=".out"

OK=1
OKVALGRIND=1
for i in $( ls $TESTDIR/*.in); do
	echo testeando $i
	ALUMNO=$i$EXT
	rm -f $ALUMNO
	CATEDRA=${i/in/catedra.out}
#	rm -f $CATEDRA
	$VALGRIND --show-reachable=yes --leak-check=yes --error-exitcode=1 -q $BINFILE -i $i -o $ALUMNO
#	$VALGRIND --show-reachable=yes --leak-check=yes --error-exitcode=1 -q $BINFILE -i $i -o $CATEDRA
	if [ $? != "0" ]; then
		OKVALGRIND=0
	fi
	diff -q $CATEDRA $ALUMNO
	if [ $? != "0" ]; then
		OK=0
	fi
done
if [ $OK != "0" ] && [ $OKVALGRIND != "0" ]; then
	echo "Tests finalizados correctamente"
fi


