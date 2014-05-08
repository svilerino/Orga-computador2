#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <cstring>

#include <vector>
#include <queue>

using namespace std;

#include "lista_colgante.h"
#include "test-o-matic.h"

#ifdef DEBUG
#define DLOG(...) printf("DEBUG:: "); printf(__VA_ARGS__);
#else
#define DLOG(...)
#endif

int isParam(char * needle, char* haystack[], int count);
char* getParam(char *needle, char *haystack[], int count);

void showusage(char* filename) {
	printf("Uso: %s [-s] -i ARCHIVO_IN -o ARCHIVO_OUT\n", filename);
	printf("\n");
	printf("Utiliza ARCHIVO como un conjunto de acciones a seguir. Cada linea del archivo de texto representa una accion a realizar. Los casos de test se separan con ---. El archivo de salida se escribira en el mismo directorio con el mismo nombre que la entrada pero extension .out.\n");
	printf("\n");
	printf("Las acciones posibles son:\n");
	printf("crear: tipo <TIPO>. valor <DATO>\n");
	printf("concatenar: nodo <numero>\n");
	printf("ahijar: nodo <numero>\n");
	printf("borrar \n");
	printf("filtrar: tipo <TIPO>\n");
	printf("colapsar: tipo <TIPO>\n");
	printf("imprimir\n");
	printf("\n");
  printf("<TIPO> corresponde a un valor del 0 si es una lista de INT, 1 si es una lista de DOUBLE y 2 si es una lista de strings.\n");
  printf("<DATO> corresponde al elemento ingresado. No se realiza un chequeo de que dato sea del tipo de la lista, por lo que es responsabilidad del que crea ARCHIVO que los datos sean del tipo de la lista.\n");
	printf("\n");
  printf("Para ver ejemplos, se puden consultar los archivos *.in provistos por la cátedra.\n");
}


int main (int argc, char *argv[])
{
	char *testin  = getParam((char*)"-i", (char**)argv, argc);
	char *testout = getParam((char*)"-o", (char**)argv, argc);

	if (!isParam((char*)"-i", argv, argc) || testin==NULL) {
		DLOG("No hay parametros\n");
		showusage(argv[0]);
		exit(-1);
	}

	bool intercalar = (bool)isParam((char*)"-s", argv, argc);

	vector<TestOMatic*> tests;
	TestOMatic::cargar(testin, testout, tests);
	if (intercalar)
		TestOMatic::correrIntercalados(tests);
	else
		TestOMatic::correrDeAUno(tests);

	TestOMatic::liberar(tests);
}


char* getParam(char *needle, char *haystack[], int count) {
	int i = 0;
	for (i = 0; i < count; i ++) {
		if (strcmp(needle, haystack[i]) == 0) {
			if (i < count -1) {
				return haystack[i+1];
			}	
		}
	}
	return 0;	
}


int isParam(char * needle, char* haystack[], int count) {
	int i = 0;
	for (i = 0; i < count; i ++) {
		if (strcmp(needle, haystack[i]) == 0) {
			return 1;
		}
	}
	return 0;	
}


