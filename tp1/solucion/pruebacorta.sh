#/bin/bash

VALGRIND=valgrind
BINFILE=./main
OKVALGRIND=1
$VALGRIND --show-reachable=yes --leak-check=yes --error-exitcode=1 -q $BINFILE
if [ $? != "0" ]; then
	OKVALGRIND=0
fi

if [ $OKVALGRIND != "0" ]; then
	echo "Test finalizado correctamente"
fi


