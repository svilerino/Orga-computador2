#include <stdlib.h>
#include <string.h>
#include "lista_colgante.h"

#define RESULTS_FILE_NAME "listaPruebaMain.txt"
#define DELETE_RESULTS_FILE_NAME_COMMAND "rm listaPruebaMain.txt"
#define SHOW_RESULTS_FILE_NAME_COMMAND "cat listaPruebaMain.txt"

void accion1();
void accion2();
void accion3();
void accion4();
void accion5();
void accion6();
void accion7();
void measureDataTypes();

int main(int argc, char *argv[])
{	
//~ Completar	
	//measureDataTypes();
	//borramos resultados anteriores
	system(DELETE_RESULTS_FILE_NAME_COMMAND);
	//1
	accion1();
	//2
	accion2();
	//3
	accion3();
	//4
	accion4();
	//5
	accion5();
	//6
	accion6();
	//7
	accion7();
	//mostramos resultados
	system(SHOW_RESULTS_FILE_NAME_COMMAND);	
	return 0;
}

void measureDataTypes(){
	printf("%%define TAM_LISTA %lu\n", sizeof(lista_colgante_t));
	printf("%%define TAM_NODO %lu\n", sizeof(nodo_t));
	printf("%%define TAM_dato_int %lu\n", sizeof(int));
	printf("%%define TAM_dato_double %lu\n", sizeof(double));
	printf("%%define TAM_dato_puntero %lu\n", sizeof(int*));
	printf("%%define TAM_value %lu\n", sizeof(valor_elemento));
	printf("%%define offset_primero %i\n", 0);//esta al principio del struct y no esta alineado por el packed
	printf("%%define offset_tipo %i\n", 0);//esta al principio del struct y no esta alineado por el packed
	printf("%%define offset_siguiente %lu\n", sizeof(tipo_elementos));
	printf("%%define offset_hijo %lu\n", sizeof(tipo_elementos) + sizeof(nodo_t*));
	printf("%%define offset_valor %lu\n", sizeof(tipo_elementos) + sizeof(nodo_t*) + sizeof(nodo_t*));	
}

void accion1(){
	//crear lista
	lista_colgante_t *lista = lista_crear();
	//imprimir lista
	char filePath[] = RESULTS_FILE_NAME;
	lista_imprimir(lista, filePath);
	//borrar lista( y sus nodos y sus char* si hay)
	lista_borrar(lista);	
}

void accion2(){
	//crear lista
	lista_colgante_t *lista = lista_crear();
	//crear valor y nodo
	valor_elemento valor;
	valor.i=10;
	nodo_t *nodo = nodo_crear(Integer, valor);
	//concatenar nodo
	lista_concatenar(lista, nodo);	
	//imprimir lista
	char filePath[] = RESULTS_FILE_NAME;
	lista_imprimir(lista, filePath);
	//borrar lista( y sus nodos y sus char* si hay)
	lista_borrar(lista);	
}

void accion3(){
	//crear lista
	lista_colgante_t *lista = lista_crear();
	//crear strings	y bla
	//crear valor y nodo
	valor_elemento valorHola;
	valorHola.s=(char*)malloc(sizeof(char)*5);
	strcpy(valorHola.s, "hola");
	nodo_t *nodoHola = nodo_crear(String, valorHola);
	//concatenar nodo
	lista_concatenar(lista, nodoHola);	
	
	valor_elemento valorChau;
	valorChau.s=(char*)malloc(sizeof(char)*5);
	strcpy(valorChau.s, "chau");
	nodo_t *nodoChau = nodo_crear(String, valorChau);
	//concatenar nodo
	lista_colgar_descendiente(lista, 0, nodoChau);	
	//imprimir lista
	char filePath[] = RESULTS_FILE_NAME;
	lista_imprimir(lista, filePath);
	//borrar lista( y sus nodos y sus char* si hay)
	lista_borrar(lista);		
}

void accion4(){
	//crear lista nodos y armar la de la figura 1 del enunciado
	lista_colgante_t *lista = lista_crear();
	valor_elemento valor;
	valor.i=-10;
	nodo_t *fst = nodo_crear(Integer, valor);
	valor.i=5;
	nodo_t *snd = nodo_crear(Integer, valor);
	valor.i=77;
	nodo_t *trd = nodo_crear(Integer, valor);
	valor.i=50;
	nodo_t *frt = nodo_crear(Integer, valor);
	
	valor.i=8;
	nodo_t *fstFstChild = nodo_crear(Integer, valor);
	valor.i=701;
	nodo_t *fstSndChild = nodo_crear(Integer, valor);
	
	valor.i=323;
	nodo_t *trdFstChild = nodo_crear(Integer, valor);
	valor.i=55;
	nodo_t *frtFstChild = nodo_crear(Integer, valor);
	
	lista_concatenar(lista, fst);
	lista_concatenar(lista, snd);
	lista_concatenar(lista, trd);
	lista_concatenar(lista, frt);
	
	lista_colgar_descendiente(lista, 0, fstFstChild);
	lista_colgar_descendiente(lista, 0, fstSndChild);
	
	lista_colgar_descendiente(lista, 2, trdFstChild);
	lista_colgar_descendiente(lista, 3, frtFstChild);	
	
	//aplicar filtrar	
	lista_filtrar_c(lista, tiene_ceros_en_decimal);
	
	//imprimir
	char filePath[] = RESULTS_FILE_NAME;
	lista_imprimir(lista, filePath);
	
	//borrar lista( y sus nodos y sus char* si hay)
	lista_borrar(lista);
}

void accion5(){
	//crear lista nodos y armar la de la figura 1 del enunciado
	lista_colgante_t *lista = lista_crear();
	valor_elemento valor;
	valor.i=-10;
	nodo_t *fst = nodo_crear(Integer, valor);
	valor.i=5;
	nodo_t *snd = nodo_crear(Integer, valor);
	valor.i=77;
	nodo_t *trd = nodo_crear(Integer, valor);
	valor.i=50;
	nodo_t *frt = nodo_crear(Integer, valor);
	
	valor.i=8;
	nodo_t *fstFstChild = nodo_crear(Integer, valor);
	valor.i=701;
	nodo_t *fstSndChild = nodo_crear(Integer, valor);
	
	valor.i=323;
	nodo_t *trdFstChild = nodo_crear(Integer, valor);
	valor.i=55;
	nodo_t *frtFstChild = nodo_crear(Integer, valor);
	
	lista_concatenar(lista, fst);
	lista_concatenar(lista, snd);
	lista_concatenar(lista, trd);
	lista_concatenar(lista, frt);
	
	lista_colgar_descendiente(lista, 0, fstFstChild);
	lista_colgar_descendiente(lista, 0, fstSndChild);
	
	lista_colgar_descendiente(lista, 2, trdFstChild);
	lista_colgar_descendiente(lista, 3, frtFstChild);	
	
	//aplicar colapsar
	lista_colapsar_c(lista, tiene_ceros_en_decimal, raiz_cuadrada_del_producto);
	
	//imprimir
	char filePath[] = RESULTS_FILE_NAME;
	lista_imprimir(lista, filePath);
	
	//borrar lista( y sus nodos y sus char* si hay)
	lista_borrar(lista);
}

void accion6(){
	//crear lista
	lista_colgante_t *lista=lista_crear();
	//poner valores double
	valor_elemento valor;
	valor.d=0.135102;
	nodo_t *fst = nodo_crear(Double, valor);
	
	valor.d=0.756891;
	nodo_t *snd = nodo_crear(Double, valor);
	
	valor.d=4.885380;
	nodo_t *trd = nodo_crear(Double, valor);
	
	valor.d=0.000215;
	nodo_t *frt = nodo_crear(Double, valor);
	
	valor.d=0.123456;
	nodo_t *fstFstChild = nodo_crear(Double, valor);
	
	valor.d=0.789456;
	nodo_t *fstSndChild = nodo_crear(Double, valor);
	
	valor.d=0.122356;
	nodo_t *trdFstChild = nodo_crear(Double, valor);
	
	lista_concatenar(lista, fst);
	lista_concatenar(lista, snd);
	lista_concatenar(lista, trd);
	lista_concatenar(lista, frt);
	
	lista_colgar_descendiente(lista, 0, fstFstChild);
	lista_colgar_descendiente(lista, 0, fstSndChild);	
	lista_colgar_descendiente(lista, 2, trdFstChild);
	
	//colapsar
	lista_colapsar_c(lista, parte_decimal_mayor_que_un_medio, raiz_de_la_suma);
	
	//imprimir
	char filePath[] = RESULTS_FILE_NAME;
	lista_imprimir(lista, filePath);
	
	//borrar lista( y sus nodos y sus char* si hay)
	lista_borrar(lista);
}

void accion7(){
	//crear lista
	lista_colgante_t *lista=lista_crear();
	//poner valores string
	valor_elemento valorFst;
	valorFst.s = (char*) malloc(sizeof(char)*6);
	strcpy(valorFst.s, "Pi0la\0");
	nodo_t *fst = nodo_crear(String, valorFst);
	
	valor_elemento valorSnd;
	valorSnd.s = (char*) malloc(sizeof(char)*6);
	strcpy(valorSnd.s, "Piola\0");
	nodo_t *snd = nodo_crear(String, valorSnd);
	
	valor_elemento valorTrd;
	valorTrd.s = (char*) malloc(sizeof(char)*8);
	strcpy(valorTrd.s, "Pi0lazo\0");
	nodo_t *trd = nodo_crear(String, valorTrd);
	
	valor_elemento valorTrdFstChild;
	valorTrdFstChild.s = (char*) malloc(sizeof(char)*8);
	strcpy(valorTrdFstChild.s, "balsamo\0");
	nodo_t *trdFstChild = nodo_crear(String, valorTrdFstChild);
	
	valor_elemento valorTrdSndChild;
	valorTrdSndChild.s = (char*) malloc(sizeof(char)*8);
	strcpy(valorTrdSndChild.s, "hora\0");
	nodo_t *trdSndChild = nodo_crear(String, valorTrdSndChild);
	
	//armar lista loca
	lista_concatenar(lista, fst);
	lista_concatenar(lista, snd);
	lista_concatenar(lista, trd);
	
	lista_colgar_descendiente(lista, 2, trdFstChild);
	lista_colgar_descendiente(lista, 2, trdSndChild); 
	
	//colapsar
	lista_colapsar_c(lista, tiene_numeros, revolver_primeras_5);
	
	//imprimir
	char filePath[] = RESULTS_FILE_NAME;
	lista_imprimir(lista, filePath);
	
	//borrar lista( y sus nodos y sus char* si hay)
	lista_borrar(lista);
}
