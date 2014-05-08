#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "lista_colgante.h"

//dissasembly cmd
//objdump -d clista_colgante.o -S clista_colgante.c -r -g -M intel

//Concatena un puntero a puntero de nodo con otro nodo
/*void nodo_concatenar(nodo_t **self, nodo_t *siguiente){	
	//asigno el siguiente al nodo *self
	(*self)->siguiente=siguiente;
}*/

/*nodo_t* nodo_crear(tipo_elementos tipo, valor_elemento valor){
	//inicializa memoria, atributos con parametros o null y devuelve referencia
	nodo_t* nodo = (nodo_t*)malloc(sizeof(nodo_t));
	nodo->tipo=tipo;
	nodo->valor=valor;
	nodo->siguiente=NULL;
	nodo->hijo=NULL;
	return nodo;
}*/

/*lista_colgante_t *lista_crear(){
	//inicializa memoria y atributos internos
	lista_colgante_t* lista = (lista_colgante_t*) malloc(sizeof(lista_colgante_t));
	lista->primero=NULL;
	return lista;
}*/

//Recibe un puntero a puntero de nodo y devuelve el ultimo de puntero a puntero de nodo
//de la cadena.
/*nodo_t** nodo_ultimo(nodo_t **self_p){
	nodo_t ** doublePtr=self_p;
	while((*self_p)!=NULL){
		doublePtr=self_p;
		self_p=&((*self_p)->siguiente);
	}
	return doublePtr;
}*/

//Devuelve el nodo base en la posicion posicion.
/*nodo_t* nodo_acceder(nodo_t *self, uint posicion){	
	uint j=0;
	nodo_t *pointer=self;
	while((self!=NULL) && (j<=posicion)){				
		pointer=self;		
		self=self->siguiente;		
		j++;
	}	
	return pointer;
}*/

//Analogo al de arriba pero mas facil de asmear
/*nodo_t* nodo_acceder(nodo_t *self, uint posicion){	
	while(posicion>0){
		self=self->siguiente;		
		posicion--;
	}
	return self;
}*/

//Auxiliares

//Borra toda una columna de nodos
/*void nodo_borrar_con_hijos(nodo_t *self){
	nodo_t *pointer=self;
	while(pointer!=NULL){
		//guardo referencia		
		nodo_t *nodoTarget = pointer;
		//avanzo hacia el hijo
		pointer=pointer->hijo;		
		//libero la string interna de ser necesario
		if(nodoTarget->tipo==String){
			free(nodoTarget->valor.s);
		}
		//libero nodo guardado
		free(nodoTarget);
	}
}*/

/*void lista_borrar(lista_colgante_t *self){
	nodo_t *pointer=self->primero;		
	while(pointer!=NULL){
		nodo_t *target=pointer;
		//avanza hacia adelante
		pointer=pointer->siguiente;
		//borra las columnas enteras
		nodo_borrar_con_hijos(target);				
	}
	//libero la lista_colgante_t
	free(self);
}*/


/*void lista_imprimir(lista_colgante_t *self, char *archivo){
	FILE* fd = fopen(archivo, "a");
	if(fd==NULL){
		fprintf(stderr, "\n\nERROR ABRIENDO EL ARCHIVO %s\n\n", archivo);
	}else{		
		printToStream(self, fd);
		fclose(fd);
	}	
}*/


//Idem anterior pero para los descendientes, usa puntero a nodo directamente.
/*nodo_t *nodo_ultimo_hijo(nodo_t *self){
	nodo_t *ptr=self;		
	while(self!=NULL){
		ptr=self;
		self=self->hijo;
	}
	return ptr;
}*/

/*void lista_concatenar(lista_colgante_t *self, nodo_t *siguiente){
	nodo_t ** nodoDblPtr = nodo_ultimo(&self->primero);	
	if((*nodoDblPtr)==NULL){
		//la lista estaba vacia...lo asigno como primero
		self->primero=siguiente;		
	}else{
		//concateno atras del nodo
		nodo_concatenar(nodoDblPtr, siguiente);
	}
}*/


/*void lista_colgar_descendiente(lista_colgante_t *self, uint posicion, nodo_t *hijo){
	//dos casos
	if(self->primero==NULL){
		//Caso 1:Lista vacia => concateno al final en vez de colgar como hijo
		lista_concatenar(self, hijo);
	}else{
		//Caso 2: cuelgo como hijo
		//obtengo el iesimo
		nodo_t * iesimoNodo = nodo_acceder(self->primero, posicion);
		//obtengo el ultimo de la columna
		nodo_t * ultimoDeLaColumna = nodo_ultimo_hijo(iesimoNodo);
		//lo cuelgo como hijo de ese ultimo
		ultimoDeLaColumna->hijo=hijo;	
	}
}*/

/*int min(int a, int b){
	return (a<b)? a : b;
}*/

//Imprime entre [ y ] en el archivo stream el valor recibido.
/*void valor_imprimir(tipo_elementos tipo, valor_elemento valor, FILE *fd){
	switch(tipo){
		case Integer:
			fprintf(fd, "[ %i ]", valor.i);				
		break;
		case Double:
			fprintf(fd, "[ %f ]", valor.d);				
		break;
		case String:
			fprintf(fd, "[ %s ]", valor.s);
		break;
	}
}*/

//Imprime entre { y } en el archivo stream la lista de los descendientes de un nodo dado.
/*void nodo_imprimir_columna(nodo_t *self, FILE *fd){
	nodo_t *hijosPtr=self;
	fprintf(fd, "{ ");	
	while(hijosPtr!=NULL){								
		valor_imprimir(hijosPtr->tipo, hijosPtr->valor, fd);
		hijosPtr=hijosPtr->hijo;	
	}
	fprintf(fd, " }");
}*/

/*void printToStream(lista_colgante_t *self, FILE* fd){	
	nodo_t *pointer=self->primero;	
	if(pointer==NULL){
		fprintf(fd, "<vacia>\n");
	}else{
		while(pointer!=NULL){			
			nodo_t *hijosPtr = pointer;
			nodo_imprimir_columna(hijosPtr, fd);
			if(pointer->siguiente!=NULL){
				fprintf(fd, " ");
			}			
			pointer=pointer->siguiente;	
		}
		fprintf(fd, "\n");
	}	
}*/

/*void valor_elemento_liberar_recursos(valor_elemento valor, tipo_elementos tipo){
	//si es String hay que liberar el char* del union
	if(tipo == String){
		free(valor.s);
	}
}*/

/*
valor_elemento valor_hacer_copia_profunda(valor_elemento valor, tipo_elementos tipo){
	//realizar una deep copy en caso de no ser string
	valor_elemento res;
	if(tipo != String){
		//son tipos primitivos, esta todo mas que piola
		res = valor;
	}else{
		//es char*, es decir String
		//si asigno puntero a puntero tengo una shallow copy del valor		
		res.s = (char *) malloc(strlen(valor.s) + 1);
		strcpy(res.s, valor.s);	
	}
	return res;
}*/

/*boolean tiene_numeros(nodo_t *n){	
	char* charIndex = n->valor.s;
	int numCount = 0;		
	while((*charIndex)!='\0'){
		//ascii value in charIndex pointer has to be in range [48..57] to be a number
		numCount += ( ( (*charIndex) >=48 ) && ( (*charIndex) <=57 ) );
		charIndex+=1;//le sumo un byte al puntero, porque los chars ocupan un byte
	}
	return (numCount>0) ? True : False;
}*/

/*valor_elemento raiz_cuadrada_del_producto(valor_elemento valorA, valor_elemento valorB){	
	valor_elemento valor;
	int res = valorA.i*valorB.i;
	valor.i = (res>0) ? (int) sqrt(res) : 0;
	return valor;
}*/

/*valor_elemento raiz_de_la_suma(valor_elemento valorA, valor_elemento valorB){
	valor_elemento valor;
	valor.d = sqrt(valorA.d+valorB.d);
	return valor;
}*/

/*boolean tiene_ceros_en_decimal(nodo_t *n){
	int value = abs(n->valor.i);
	if(value==0) return False;	
	int zeroCount=0;
	while(value>0){
		zeroCount += ((value%10)==0);
		value=(int) (value/10);
	}	
	return (zeroCount>0) ? True : False;
}*/

/*boolean parte_decimal_mayor_que_un_medio(nodo_t *n){
	double x=fabs(n->valor.d);			
	return ( (x - floor(x)) > 0.5) ? True : False;
}*/

/*void lista_filtrar(lista_colgante_t *self, nodo_bool_method test_method){
	if(self->primero != NULL){
		nodo_t ** nodo = &(self->primero);
		while((*nodo)!=NULL){
			if(test_method(*nodo)){
				nodo_t *toDelete = *nodo;
				*nodo=(*nodo)->siguiente;
				nodo_borrar_con_hijos(toDelete);
			}else{
				nodo=&((*nodo)->siguiente);
			}
		}
	}
}*/

/*void lista_colapsar(lista_colgante_t *self, nodo_bool_method test_method, nodo_value_method join_method){
	//verifico que no haga bumba bumba raka taka taka bum el &(self->primero)
	if(self->primero != NULL){
		//me guardo la referencia doble para poder cambiar el puntero que me apunta
		//siendo el actual si tengo que hacer estas cosas
		nodo_t **current = &(self->primero);		
		//itero con el pointer
		while(*current != NULL){
			//testeo el nodo, notar que como es doble * va con desreferencia
			if(test_method(*current) == True){	
				//si entro aca es porque el nodo cumplio la condicion de test_method
				//entonces decimos "let the game begin" y colapsamos la columna
				nodo_colapsar_c(current, join_method);
			}
			//avanzamos
			current = &((*current)->siguiente);			
		}
	}
}*/

//Recibe un puntero a puntero a un nodo base y se encarga de colapsar todos sus descendientes.
/*void nodo_colapsar(nodo_t **self_pointer, nodo_value_method join_method){
	//guardo referencia al nodo
	nodo_t *nodo = *self_pointer;
	//me hago una deep copy del valor del nodo base(si es string hay que copiar y alocar nuevamente)
	valor_elemento valor = valor_hacer_copia_profunda(nodo->valor, nodo->tipo);
	//me creo el nuevo nodo base
	nodo_t *nuevo = nodo_crear(nodo->tipo, valor);
	nodo_t *hijo = nodo->hijo;
	//voy joineando la columna
	while(hijo != NULL){
		//obtengo el resultado de joinear el valor del ciclo actual de nuevo con el siguiente hijo no procesado
		valor_elemento resultado = join_method(nuevo->valor, hijo->valor);
		//libero de ser necesario el valor anterior de nuevo antes de reemplazarlo
		//por el nuevo obtenido de joinear el hijo actual
		valor_elemento_liberar_recursos(nuevo->valor, nodo->tipo);
		//reemplazo el valor
		nuevo->valor = resultado;
		//avanzo en la columna hacia abajo
		hijo = hijo->hijo;
	}
	//concateno el siguiente del nodo original con el nuevo colapsado
	nuevo->siguiente = nodo->siguiente;
	//concateno a quien apuntaba al original, el nodo nuevo colapsado
	*self_pointer = nuevo;
	//elimino y libero recursos del nodo original, cuya columna fue colapsada y no
	//es necesaria, not anymore.
	nodo_borrar_con_hijos(nodo);
}*/

/*valor_elemento revolver_primeras_5(valor_elemento vA, valor_elemento vB){	
	//creo copias de los punteros de los parametros
	char *x = vA.s;
	char *y = vB.s;
	//se agrega uno mas para el caracter nulo \0 que indica la terminacion de la cadena
	//declaro 3 variables auxiliares para utilizar como indices en el merge
	int i = 0;//corresponde al indice de x
	int j = 0;//corresponde al indice de y
	int k = 0;//corresponde al indice de z
	//declaro 3 variables como limites de iteracion
	int kLimit = 10;
	int iLimit = strlen(x);
	int jLimit = strlen(y);	
	//asigno espacio en heap para kLimit+1(\0) char*'s	*/
	//char *z = (char *) malloc(kLimit+1/* *sizeof(char*) da uno en la arquitectura*/);
	
	/*//inicializo la porcion de memoria iniciada con base en z y 11 bytes despues con \0
	memset(z, '\0', kLimit+1);
	//simulo turnos con un contador modular para saber que string concatenar y que indice mover
	int modCount = 0;
	//hago una variacion del merge para listas ordenadas, pero tomando modCount como criterio
	while(k < kLimit && i < iLimit && j < jLimit){
		//concateno el correspondiente
		if(modCount == 0){
			z[k] = x[i];
			i++;
		}else{
			z[k] = y[j];			
			j++;
		}
		//avanzo indice salida y cambio turno
		k++;
		modCount=(modCount+1)%2;
	}
	
	//si quedo agotada la string a, completo hasta el final con b
	if(i == iLimit){
		while(k < kLimit && j < jLimit){
			z[k] = y[j];
			j++;
			k++;
		}
	}
	//si quedo agotada la string b, completo hasta el final con a
	if(j == jLimit){
		while(k < kLimit && i < iLimit){
			z[k] = x[i];
			k++;
			i++;
		}
	}
	//wrappeo en valor_resultado el merge
	valor_elemento resultado;
	resultado.s = z;
	return resultado;
}*/

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//----------------------- Funciones de C --------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------


void lista_colapsar_c(lista_colgante_t *self, nodo_bool_method test_method, nodo_value_method join_method){
	//verifico que no haga bumba bumba raka taka taka bum el &(self->primero)
	if(self->primero != NULL){
		//me guardo la referencia doble para poder cambiar el puntero que me apunta
		//siendo el actual si tengo que hacer estas cosas
		nodo_t **current = &(self->primero);		
		//itero con el pointer
		while(*current != NULL){
			//testeo el nodo, notar que como es doble * va con desreferencia
			if(test_method(*current) == True){	
				//si entro aca es porque el nodo cumplio la condicion de test_method
				//entonces decimos "let the game begin" y colapsamos la columna
				nodo_colapsar_c(current, join_method);
			}
			//avanzamos
			current = &((*current)->siguiente);			
		}
	}
}

void lista_filtrar_c(lista_colgante_t *self, nodo_bool_method test_method){
	if(self->primero != NULL){
		nodo_t ** nodo = &(self->primero);
		while((*nodo)!=NULL){
			if(test_method(*nodo)){
				nodo_t *toDelete = *nodo;
				*nodo=(*nodo)->siguiente;
				nodo_borrar_con_hijos(toDelete);
			}else{
				nodo=&((*nodo)->siguiente);
			}
		}
	}
}

//Recibe un puntero a puntero a un nodo base y se encarga de colapsar todos sus descendientes.
void nodo_colapsar_c(nodo_t **self_pointer, nodo_value_method join_method){
	//guardo referencia al nodo
	nodo_t *nodo = *self_pointer;
	//me hago una deep copy del valor del nodo base(si es string hay que copiar y alocar nuevamente)
	valor_elemento valor = valor_hacer_copia_profunda(nodo->valor, nodo->tipo);
	//me creo el nuevo nodo base
	nodo_t *nuevo = nodo_crear(nodo->tipo, valor);
	nodo_t *hijo = nodo->hijo;
	//voy joineando la columna
	while(hijo != NULL){
		//obtengo el resultado de joinear el valor del ciclo actual de nuevo con el siguiente hijo no procesado
		valor_elemento resultado = join_method(nuevo->valor, hijo->valor);
		//libero de ser necesario el valor anterior de nuevo antes de reemplazarlo
		//por el nuevo obtenido de joinear el hijo actual
		valor_elemento_liberar_recursos(nuevo->valor, nodo->tipo);
		//reemplazo el valor
		nuevo->valor = resultado;
		//avanzo en la columna hacia abajo
		hijo = hijo->hijo;
	}
	//concateno el siguiente del nodo original con el nuevo colapsado
	nuevo->siguiente = nodo->siguiente;
	//concateno a quien apuntaba al original, el nodo nuevo colapsado
	*self_pointer = nuevo;
	//elimino y libero recursos del nodo original, cuya columna fue colapsada y no
	//es necesaria, not anymore.
	nodo_borrar_con_hijos(nodo);
}
