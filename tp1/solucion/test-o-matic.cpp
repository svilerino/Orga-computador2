#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>

#include "test-o-matic.h"

#include <iostream>

using namespace std;

void debio_leer(int cantidad, int leido, FILE *fp)
{
	if (cantidad == leido) return;
	char err_str[200], str[100];
	fgets(str, 100, fp);
	sprintf(err_str, "No se pudo leer el test a partir de >%s", str);
	perror (err_str);
	assert(false);
}

AccionCrear::AccionCrear(FILE *fp)
{
	char str[100]; size_t size;
	int leido = fscanf(fp, " tipo %d. ", (int*)&tipo);
	debio_leer(1, leido, fp);
	switch(tipo) {
		case Integer: leido = fscanf(fp, "valor %d", &this->valor.i); break;
		case Double:  leido = fscanf(fp, "valor %lf", &this->valor.d); break;
		case String: 	leido = fscanf(fp, "valor %s", (char*)&str); size = strlen(str)+1;
			valor.s = (char*)malloc(size);
			strcpy(valor.s, str); break;
		default: break;
	}

	debio_leer(1, leido, fp);
}
AccionCrear::AccionCrear(tipo_elementos t, valor_elemento v) : tipo(t), valor(v) {}

void AccionCrear::escribir()
{
	fprintf(stdout, "crear: tipo %d. ", tipo);
	switch(tipo) {
		case Integer: fprintf(stdout, "valor %d\n", valor.i); break;
		case Double:  fprintf(stdout, "valor %f\n", valor.d); break;
		case String: 	fprintf(stdout, "valor %s\n", valor.s); break;
		default: break;
	}
}


AccionCrear::~AccionCrear()
{

}


void AccionCrear::ejecutar()
{
	nodo_t *nuevo = nodo_crear(tipo, valor);	
	test->libres.push_back(nuevo);
}

AccionBorrar::AccionBorrar(FILE *fp)
{

}

void AccionBorrar::escribir()
{
	fprintf(stdout, "borrar\n");
}


void AccionBorrar::ejecutar()
{
	lista_borrar(test->lista);
}


AccionImprimir::AccionImprimir(FILE *fp)
{
	fgets(str, 100, fp);
	char *sin = str;
	while (*sin != '\n' && (*sin == '\t' || *sin == ' '))
		sin++;
	
	char *sout = str;
	while (*sin != '\n')
	{
		*sout = *sin;	sin++; sout++;
	}
	if (sout != str) { *sout = ' '; sout++;} // si tiene al menos un caracter
	*sout = 0;

}

void AccionImprimir::ejecutar()
{
	FILE *stream = fopen(test->archivo_salida, "a");
	fprintf(stream, "%s", str);
	fclose(stream);
	lista_imprimir(test->lista, test->archivo_salida);
}

void AccionImprimir::escribir()
{
	fprintf(stdout, "imprimir\n");
}

AccionConcatenar::AccionConcatenar(FILE *fp)
{
	int leido = fscanf(fp, " nodo %u", &con_nodo);
	debio_leer(1, leido, fp);
}

void AccionConcatenar::escribir()
{
	fprintf(stdout, "concatenar: nodo %u\n", con_nodo);
}

void AccionConcatenar::ejecutar()
{
	nodo_t *destino = test->libres[con_nodo];
	lista_concatenar(test->lista, destino);
	//test->libres.erase(test->libres.begin()+con_nodo);
}

AccionAhijar::AccionAhijar(FILE *fp)
{
	int leido = fscanf(fp, " nodo %u en columna %u", &nodo, &en_columna);
	debio_leer(2, leido, fp);
}

void AccionAhijar::escribir()
{
	fprintf(stdout, "ahijar: nodo %u en columna %u\n", nodo, en_columna);
}

uint nodo_longitud(nodo_t *self)
{
	uint i = 0;
	while (self != NULL)
	{
		self = self->siguiente; i++;
	}
	return i;
}


void AccionAhijar::ejecutar()
{
	uint longitud = nodo_longitud(test->lista->primero);
	uint columna = 0;
	if (longitud != 0) columna = en_columna % longitud;
	nodo_t *destino = test->libres[nodo];
	lista_colgar_descendiente(test->lista, columna, destino);
	//test->libres.erase(test->libres.begin()+con_nodo);
}


AccionFiltrar::AccionFiltrar(FILE *fp)
{
	int leido = fscanf(fp, " tipo %u", (uint*)&tipo);
	debio_leer(1, leido, fp);
}

void AccionFiltrar::escribir()
{
	fprintf(stdout, "imprimir: antes   de filtrar:\n");
	fprintf(stdout, "filtrar: tipo %u\n", tipo);
	fprintf(stdout, "imprimir: despues de filtrar:\n");
}

void AccionFiltrar::ejecutar()
{
	nodo_bool_method test_meth;
	switch (tipo) {
		case Integer: test_meth = tiene_ceros_en_decimal; break;
		case Double:  test_meth = parte_decimal_mayor_que_un_medio; break;
		case String:  test_meth = tiene_numeros; break;
	}

	lista_filtrar(test->lista, test_meth);
}

AccionColapsar::AccionColapsar(FILE *fp)
{
	int leido = fscanf(fp, " tipo %u", (uint*)&tipo);
	debio_leer(1, leido, fp);
}

void AccionColapsar::escribir()
{
	fprintf(stdout, "imprimir: antes   de colapsar\n");
	fprintf(stdout, "colapsar: tipo %u\n", tipo);
	fprintf(stdout, "imprimir: despues de colapsar\n");
}


void AccionColapsar::ejecutar()
{
	nodo_bool_method test_meth;
	nodo_value_method value;
	switch (tipo) {
		case Integer: test_meth = tiene_ceros_en_decimal;  value = raiz_cuadrada_del_producto; break;
		case Double:  test_meth = parte_decimal_mayor_que_un_medio; value = raiz_de_la_suma;      break;
		case String:  test_meth = tiene_numeros;           value = revolver_primeras_5; break;
	}

	lista_colapsar(test->lista, test_meth, value);
}


TestReader::TestReader(const char *archivo_in, const char *archivo_out, vector<TestOMatic*> &tests) {
	FILE *fp = fopen(archivo_in, "r");
  if (fp == NULL)
		perror("No se pudo abrir el archivo de test");

	char str[100];
	size_t str_size = 100;
	while (fscanf(fp, "%s", (char*)str) != EOF)
	{
		if (str[0] == '-')
		{
			TestOMatic *test = new TestOMatic;
			strcpy((char*)test->archivo_salida,archivo_out);
			tests.push_back(test);
			fgets(str, str_size, fp);

		}
		else
			cargar_accion(fp, (char*)str, *tests.back());
	}

	fclose(fp);
}


TestReader::~TestReader()
{
}

enum AccionToken { CrearToken, ConcatenarToken, ColapsarToken,
									 AhijarToken, ImprimirToken, BorrarToken, FiltrarToken, ErrorToken };

AccionToken tokenizar(char *accion)
{
	switch (accion[0]) {
		case 'c': 
			if (accion[1] == 'r') return CrearToken;
			else if	(accion[2] == 'n')	return ConcatenarToken;
			else return ColapsarToken; break;
		case 'a': return AhijarToken; break;
		case 'i': return ImprimirToken; break;
		case 'b': return BorrarToken; break;
		case 'f': return FiltrarToken; break;
		default: perror("accion desconocida"); printf("%s", accion);
	}
	return ErrorToken;
}

void TestReader::cargar_accion(FILE *fp, char *accion_str, TestOMatic &test)
{
	AccionTest *accion = NULL;
	AccionToken token = tokenizar(accion_str);
	switch (token) {
		case CrearToken:      accion = new AccionCrear(fp); break;
		case ConcatenarToken: accion = new AccionConcatenar(fp); break;
		case ColapsarToken:   accion = new AccionColapsar(fp);	break;
		case AhijarToken:     accion = new AccionAhijar(fp); break;
		case ImprimirToken:   accion = new AccionImprimir(fp); break;
		case BorrarToken:     accion = new AccionBorrar(fp); break;
		case FiltrarToken:    accion = new AccionFiltrar(fp); break;
		default: perror("accion no implementada"); printf("%s", accion_str);
	}

	if (accion != NULL)
		test.agregar_accion(accion);
	else
		perror("Error parseando accion");
}


void TestOMatic::cargar(const char *archivo_in, const char *archivo_out, vector<TestOMatic*> &tests)
{
	TestReader reader(archivo_in, archivo_out, tests);
}

void TestOMatic::liberar(std::vector<TestOMatic*> &tests)
{
  for (size_t i = 0; i < tests.size(); i++)
			delete tests[i];
}

void TestOMatic::correrDeAUno(std::vector<TestOMatic*> &tests)
{
  for (size_t i = 0; i < tests.size(); i++)
		tests[i]->correr();
}

void TestOMatic::correrIntercalados(std::vector<TestOMatic*> &tests)
{
	size_t cantidad = 0;
  for (size_t i = 0; i < tests.size(); i++)
		cantidad = max(cantidad, tests[i]->cantAcciones() );

	for (size_t i = 0; i < cantidad; i++)
		for (size_t j = 0; j < tests.size(); j++)
			tests[j]->correr(i);
}

TestOMatic::~TestOMatic()
{
	for (size_t i = 0; i < acciones.size(); i++)
		delete acciones[i];
}

void TestOMatic::correr()
{
	for (size_t i = 0; i < acciones.size(); i++)
		correr(i);
}

void TestOMatic::correr(size_t i)
{
	if (i < acciones.size())
		acciones[i]->ejecutar();	
}




