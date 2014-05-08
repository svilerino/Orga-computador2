#/bin/bash

function mostrar_uso {
	echo " *** script para testear de 1 archivo de test *** "
  echo " "
  echo "pruebacorta.sh [--stdout] [-c] <archivo_in>"
  echo " "
  echo "opciones:"
  echo "-h, --help                mostrar esta ayuda"
  echo "-c,                       comparar salida con la salida de la catedra"
	echo "--stdout                  escribir en el archivo de salida estandar (en la consola)"
  exit 0
}

COMPARAR=0
STDOUT=0
while test $# -gt 1; do
    case "$1" in
        -h|--help)
            mostrar_uso
            ;;
        -c)
            shift
            COMPARAR=1
            ;;
				--stdout)
            shift
						STDOUT=1
						;;
        *)
            break
            ;;
    esac
done          

VALGRIND=valgrind
BINFILE=./tester
EXT=".out"

OK=1
OKVALGRIND=1

ARCHIVO_TEST=$1
if [ -z $ARCHIVO_TEST ]; then
	echo no se especifico un archivo de entrada
	mostrar_uso
fi
if [ ! -f $ARCHIVO_TEST ]; then
	echo el archivo de entrada no existe
	mostrar_uso
fi

echo testeando $ARCHIVO_TEST
if [ $STDOUT != "0" ]; then
	ALUMNO="/dev/stdout"
else
	ALUMNO=$ARCHIVO_TEST$EXT
	rm -f $ALUMNO
fi

echo $VALGRIND --show-reachable=yes --leak-check=yes --error-exitcode=1 -q $BINFILE -i $ARCHIVO_TEST -o $ALUMNO
$VALGRIND --show-reachable=yes --leak-check=yes --error-exitcode=1 -q $BINFILE -i $ARCHIVO_TEST -o $ALUMNO
if [ $? != "0" ]; then
	OKVALGRIND=0
fi

CATEDRA=${ARCHIVO_TEST/in/catedra.out}


if [ $COMPARAR != "0" ]; then
	diff -q $CATEDRA $ALUMNO
	if [ $? != "0" ]; then
		OK=0
	fi
fi

if [ $OK != "0" ] && [ $OKVALGRIND != "0" ]; then
	echo "Test finalizado correctamente"
fi


