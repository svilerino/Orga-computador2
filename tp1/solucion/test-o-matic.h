
#ifndef _TEST_O_MATIC_H_
#define _TEST_O_MATIC_H_

#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <cstring>

#include <vector>
#include <queue>

#include "lista_colgante.h"


class TestOMatic;

class AccionTest { public:
	virtual void ejecutar() = 0;
	virtual ~AccionTest() {};
	virtual void escribir() = 0;
public:
	TestOMatic *test;
};

class AccionCrear : public AccionTest { public:
	AccionCrear(FILE *fp);
	AccionCrear(tipo_elementos t, valor_elemento v);
	~AccionCrear();

	virtual void ejecutar();
	virtual void escribir();
	tipo_elementos tipo;
	valor_elemento valor;
};

class AccionBorrar : public AccionTest { public:
	AccionBorrar(FILE *fp);
	AccionBorrar() {};

	virtual void ejecutar();
	virtual void escribir();
};


class AccionImprimir : public AccionTest { public:
	AccionImprimir(FILE *fp);
	AccionImprimir(){};

	virtual void ejecutar();
	virtual void escribir();
	char str[100];
};

class AccionConcatenar : public AccionTest { public:
	AccionConcatenar(FILE *fp);
	AccionConcatenar(uint n) : con_nodo(n) {};
	virtual void ejecutar();
	virtual void escribir();

	uint con_nodo;
};

class AccionAhijar : public AccionTest { public:
	AccionAhijar(FILE *fp);
	AccionAhijar(uint n, uint col) : nodo(n), en_columna(col) {};
	virtual void ejecutar();
	virtual void escribir();

	uint nodo;
	uint en_columna;
};

class AccionFiltrar : public AccionTest { public:
	AccionFiltrar(FILE *fp);
	AccionFiltrar(tipo_elementos t) : tipo(t){};
	virtual void ejecutar();
	virtual void escribir();

	tipo_elementos tipo;
};

class AccionColapsar : public AccionTest { public:
	AccionColapsar(FILE *fp);
	AccionColapsar(tipo_elementos t) : tipo(t) {};
	virtual void ejecutar();
	virtual void escribir();

	tipo_elementos tipo;
};

class TestReader;

class TestOMatic {

public:
		TestOMatic(const TestOMatic &copy) : libres(copy.libres), lista(copy.lista), acciones(copy.acciones) {};	
		TestOMatic() {lista = lista_crear(); }
		~TestOMatic();

		static void cargar(const char *archivo_in, const char *archivo_out, std::vector<TestOMatic*> &tests);
		static void correrIntercalados(std::vector<TestOMatic*> &tests);
		static void correrDeAUno(std::vector<TestOMatic*> &tests);
		static void liberar(std::vector<TestOMatic*> &tests);

		void correr();
		void correr(size_t i);

		void agregar_accion(AccionTest *accion) { acciones.push_back(accion); accion->test = this; }
		
		std::vector<nodo_t*> libres;
		lista_colgante_t *lista;
		char archivo_salida[100];
		size_t cantAcciones() { return acciones.size();}
private:


	std::vector<AccionTest*> acciones;
};


class TestReader {
public:
	TestReader(const char *archivo_in, const char *archivo_out, std::vector<TestOMatic*> &tests);
	void cargar_accion(FILE *fp, char *accion, TestOMatic &test);
	~TestReader();
	
private:
	
};


#endif /// _TEST_O_MATIC_H_

