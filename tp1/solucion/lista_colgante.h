
#ifndef _MATRIZ_RALA_H_
#define _MATRIZ_RALA_H_

#include <stdio.h>

#include "base.h"

#ifdef __cplusplus
extern "C" {
#endif

struct elemento;

typedef struct lista_colgante_t {
	struct nodo_t *primero;
} __attribute__((__packed__)) lista_colgante_t;


typedef struct nodo_t {
	tipo_elementos tipo;
	struct nodo_t *siguiente;
	struct nodo_t *hijo;
	valor_elemento valor;
} __attribute__((__packed__)) nodo_t;


typedef nodo_t* lista_elemento_p;

typedef boolean        (*nodo_bool_method) (nodo_t* self);
typedef valor_elemento (*nodo_value_method)(valor_elemento vA, valor_elemento vB);


//~ Basicas

lista_colgante_t *lista_crear();
void lista_borrar(lista_colgante_t *self);

nodo_t* nodo_crear(tipo_elementos tipo, valor_elemento valor);

void lista_concatenar(lista_colgante_t *self, nodo_t *siguiente);
void lista_colgar_descendiente(lista_colgante_t *self, uint posicion, nodo_t *hijo);

void lista_imprimir(lista_colgante_t *self, char *archivo);
//auxiliar
void printToStream(lista_colgante_t *self, FILE* stream);

//~ Auxiliares
void lista_borrar_nodos(nodo_t *self);
void nodo_borrar_con_hijos(nodo_t *self);
uint nodo_longitud(nodo_t *self);
uint nodo_altura(nodo_t *self);
nodo_t* nodo_acceder(nodo_t *self, uint distancia);
void nodo_concatenar(nodo_t **self, nodo_t *siguiente);
int min(int, int);


//~ Complejas

void lista_filtrar(lista_colgante_t *self, nodo_bool_method test_method);
void lista_colapsar(lista_colgante_t *self, nodo_bool_method test_method, nodo_value_method join_method);

void lista_filtrar_c(lista_colgante_t *self, nodo_bool_method test_method);
void lista_colapsar_c(lista_colgante_t *self, nodo_bool_method test_method, nodo_value_method join_method);

//~ Sencillas

boolean tiene_ceros_en_decimal(nodo_t *n);
boolean parte_decimal_mayor_que_un_medio(nodo_t *n);
boolean tiene_numeros(nodo_t *n);

valor_elemento raiz_cuadrada_del_producto(valor_elemento valorA, valor_elemento valorB);
valor_elemento raiz_de_la_suma(valor_elemento valorA, valor_elemento valorB);
valor_elemento revolver_primeras_5(valor_elemento vA, valor_elemento vB);

//Auxiliares requeridas por el enunciado

//Borra toda una columna de nodos
void nodo_borrar_con_hijos(nodo_t *self);
//Devuelve el nodo base en la posicion posicion.
nodo_t* nodo_acceder(nodo_t *self, uint posicion);
//Concatena un puntero a puntero de nodo con otro nodo
void nodo_concatenar(nodo_t **self, nodo_t *siguiente);
//Recibe un puntero a puntero de nodo y devuelve el ultimo de puntero a puntero de nodo
//de la cadena.
nodo_t** nodo_ultimo(nodo_t **self_p);
//Idem anterior pero para los descendientes, usa puntero a nodo directamente.
nodo_t *nodo_ultimo_hijo(nodo_t *self);
//Imprime entre [ y ] en el archivo stream el valor recibido.
void valor_imprimir(tipo_elementos tipo, valor_elemento valor, FILE *stream);
//Imprime entre { y } en el archivo stream la lista de los descendientes de un nodo dado.
void nodo_imprimir_columna(nodo_t *self, FILE *stream);
//Recibe un puntero a puntero a un nodo base y se encarga de colapsar todos sus descendientes.
void nodo_colapsar(nodo_t **self_pointer, nodo_value_method join_method);
//Recibe un puntero a puntero a un nodo base y se encarga de colapsar todos sus descendientes.
void nodo_colapsar_c(nodo_t **self_pointer, nodo_value_method join_method);

void valor_elemento_liberar_recursos(valor_elemento valor, tipo_elementos tipo);
valor_elemento valor_hacer_copia_profunda(valor_elemento valor, tipo_elementos tipo);

#ifdef __cplusplus
}
#endif

#endif /// _MATRIZ_RALA_H_


