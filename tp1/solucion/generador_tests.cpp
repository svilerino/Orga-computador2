
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <vector>
#include <algorithm> // random_shuffle

#include "test-o-matic.h"

//#define SALIDATESTS "../tests/test%.2d.in"
#define SALIDATESTS "../testsAlumno/test%.2d.in"
/*#define CANTIDAD_DE_TESTS 50
#define CANTIDAD_DE_LISTAS_POR_TEST 5
#define CANTIDAD_DE_NODOS_POR_LISTA 50
#define CANTIDAD_DE_NODOS_CONCATENADOS 15
#define CANTIDAD_DE_NODOS_AHIJADOS ( CANTIDAD_DE_NODOS_POR_LISTA - CANTIDAD_DE_NODOS_CONCATENADOS )
#define CANTIDAD_DE_FILTROS 3
#define CANTIDAD_DE_COLAPSOS 7
#define MAX_STRING_LENGTH 10*/

#define CANTIDAD_DE_TESTS 50
#define CANTIDAD_DE_LISTAS_POR_TEST 50
#define CANTIDAD_DE_NODOS_POR_LISTA 100
#define CANTIDAD_DE_NODOS_CONCATENADOS 40
#define CANTIDAD_DE_NODOS_AHIJADOS ( CANTIDAD_DE_NODOS_POR_LISTA - CANTIDAD_DE_NODOS_CONCATENADOS )
#define CANTIDAD_DE_FILTROS 10
#define CANTIDAD_DE_COLAPSOS 0
#define MAX_STRING_LENGTH 10


using namespace std;

valor_elemento ival(int i) { valor_elemento v; v.i = i; return v; }
valor_elemento dval(double d) { valor_elemento v; v.d = d; return v; }
valor_elemento sval(char *s) { valor_elemento v; v.s = s; return v; }


// incluyendo to [from, to]
int aleatorio(int from, int to)
{
	int total = to + 1 - from;
	return (rand() % total) + from;
}

valor_elemento generar_valor_aleatorio(tipo_elementos tipo_valor)
{
	switch (tipo_valor)
	{
	case Integer:
		return ival((rand()%10000)-5000);
	case Double:
		return dval(((rand()-(RAND_MAX/4))/(RAND_MAX+1.0))*100);
	case String:
		int len = aleatorio(1, MAX_STRING_LENGTH) + 1;
		char *result = (char*)malloc(len);
		for(int i=0;i<len;i++) {
			result[i] = rand()%35; 
			if (result[i] < 25)
				 result[i] += 'a';
			else result[i] += '0' - 25;
		}
		result[len-1] = 0;
		return sval(result);
	}
	valor_elemento v;
	return v;
}

AccionTest* generar_operacion_crear(tipo_elementos tipo_valor)
{
	return new AccionCrear(tipo_valor, generar_valor_aleatorio(tipo_valor));
}

AccionTest* generar_operacion_concatenar(uint num)
{
	return new AccionConcatenar(num);
}

AccionTest* generar_operacion_ahijar(uint num, uint col)
{
	return new AccionAhijar(num, col);
}

AccionTest* generar_operacion_imprimir()
{
	return new AccionImprimir();
}

AccionTest* generar_operacion_borrar()
{
	return new AccionBorrar();
}

AccionTest* generar_operacion_filtrar(tipo_elementos tipo)
{
	return new AccionFiltrar(tipo);
}

AccionTest* generar_operacion_colapsar(tipo_elementos tipo)
{
	return new AccionColapsar(tipo);
}

void generar_operaciones()
{
	vector<AccionTest*> operaciones;
	tipo_elementos tipo = tipo_elementos(rand()%3);
	
	vector<int> usos;
	for (int i = 0; i < CANTIDAD_DE_NODOS_CONCATENADOS; i++)
		usos.push_back(1);
	for (int i = 0; i < CANTIDAD_DE_NODOS_AHIJADOS ; i++)
		usos.push_back(0);
	random_shuffle(usos.begin()+1, usos.end());
	
	vector<int> disponibles;
	int creados = 0, usados = 0;
	for (int i = 0; i < CANTIDAD_DE_NODOS_POR_LISTA*2; i++)
	{
		int a = aleatorio(0,1);
		if ((creados > usados && a) || creados == CANTIDAD_DE_NODOS_POR_LISTA)
		{
			int ind = aleatorio(0, disponibles.size()-1);
			int tipo = usos.back(); usos.pop_back();
			int nodo = disponibles[ind]; disponibles.erase(disponibles.begin()+ind);
			
			if (tipo == 0) operaciones.push_back(generar_operacion_concatenar(nodo));
			else operaciones.push_back(generar_operacion_ahijar(nodo, (uint)aleatorio(0, CANTIDAD_DE_NODOS_POR_LISTA-1)));
			usados++;
		}
		else
		{
			operaciones.push_back(generar_operacion_crear(tipo));
			disponibles.push_back(creados);
			creados++;
		}
	}	
		
	for (int i = 0; i < CANTIDAD_DE_FILTROS; i++)
	{
		int a = aleatorio(0, operaciones.size());
		operaciones.insert(operaciones.begin()+a, generar_operacion_filtrar(tipo));
	}

	for (int i = 0; i < CANTIDAD_DE_COLAPSOS; i++)
	{
		int a = aleatorio(0, operaciones.size());
		operaciones.insert(operaciones.begin()+a, generar_operacion_colapsar(tipo));
	}
		
	operaciones.push_back(generar_operacion_borrar());
	for (size_t i = 0; i < operaciones.size(); i++)
		operaciones[i]->escribir();
}

void generar_archivo_de_test(int numero)
{
	char filename[100];
	
	sprintf(filename,SALIDATESTS,numero);
	freopen(filename,"w",stdout);
 
	for (int i = 0; i < CANTIDAD_DE_LISTAS_POR_TEST; i++) {
		fprintf(stdout, "--- test %d de %d\n", i, CANTIDAD_DE_TESTS);
		generar_operaciones();
	}
	
}
	
	
int main(int argc, char *argv[])
{
	srand(31414);
	
	for (int i = 0; i < CANTIDAD_DE_TESTS; i++)
		generar_archivo_de_test(i);
}


