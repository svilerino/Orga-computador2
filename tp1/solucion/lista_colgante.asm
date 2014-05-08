global lista_crear
global nodo_crear
global nodo_concatenar
global nodo_ultimo
global nodo_acceder
global nodo_borrar_con_hijos
global lista_borrar
global nodo_imprimir_columna
global nodo_ultimo_hijo
global lista_imprimir
global lista_concatenar
global lista_colgar_descendiente
global min
global printToStream
global valor_imprimir
global valor_elemento_liberar_recursos
global valor_hacer_copia_profunda
global tiene_numeros
global raiz_de_la_suma
global raiz_cuadrada_del_producto
global tiene_ceros_en_decimal
global parte_decimal_mayor_que_un_medio
global lista_filtrar
global lista_colapsar
global nodo_colapsar
global revolver_primeras_5

;----------------extern libc
extern malloc
extern free
extern fprintf
extern printf
extern fopen
extern fclose
extern strlen
extern strcpy
extern fabs
extern memset

%define TAM_LISTA 8
%define TAM_NODO 28
%define TAM_dato_int 4
%define TAM_dato_double 8
%define TAM_dato_puntero 8
%define TAM_value 8
%define offset_primero 0
%define offset_tipo 0
%define offset_siguiente 4
%define offset_hijo 12
%define offset_valor 20

%define ENUM_int 0
%define ENUM_double 1
%define ENUM_string 2

%define False 0
%define True 1

%define NULL 0

section .data

formatoPrintfString: DB '[ %s ]', 0
formatoPrintfInteger: DB '[ %i ]', 0
formatoPrintfDouble: DB '[ %f ]', 0
formatoOpenBracket: DB '{ ', 0
formatoCloseBracket: DB ' }', 0
fileopenMode: DB 'a',0
fileopenError: DB 10, 10, 'ERROR ABRIENDO EL ARCHIVO %s', 10, 10, 0
emptyListLegend: DB '<vacia>', 10, 0
constanteEnter: DB 10, 0
constanteEspacio: DB ' ', 0
oneHalf: DQ 0.5

section .text
; ~ lista* lista_crear()
;	//inicializa memoria y atributos internos
;	lista_colgante_t* lista = (lista_colgante_t*) malloc(sizeof(lista_colgante_t));
;	lista->primero=NULL;
;	return lista;
lista_crear:
	MOV RDI, 0
	MOV EDI, TAM_LISTA
	SUB RSP, 8;alineo
	CALL malloc
	ADD RSP, 8
	;inicializo en NULL primero
	MOV qword [RAX+offset_primero], NULL
	;en RAX esta el resultado de reservaMemoria que es lo mismo que queremos devolver, so, piola
	ret

; ~ nodo_t* nodo_crear(tipo_elementos tipo, valor_elemento value)
;//inicializa memoria, atributos con parametros o null y devuelve referencia
	;nodo_t* nodo = (nodo_t*)malloc(sizeof(nodo_t));
	;nodo->tipo=tipo;
	;nodo->valor=valor;
	;nodo->siguiente=NULL;
	;nodo->hijo=NULL;
	;return nodo;
nodo_crear:
	PUSH R12
	PUSH R13
	MOV R12, 0
	MOV R12D, EDI; aca esta tipo_elementos 4 bytes!
	MOV R13, RSI; aca esta valor_elemento 8 bytes!
	MOV RDI, 0
	MOV EDI, TAM_NODO
	CALL malloc
	;inicializo los atributos del struct packed
	;tipo
	MOV dword [RAX+offset_tipo], R12d;OJO QUE ESTO SON 4 BYTES NOMAS!!
	;valor
	MOV qword [RAX+offset_valor], R13
	;siguiente
	MOV qword [RAX+offset_siguiente], NULL
	;hijo
	MOV qword [RAX+offset_hijo], NULL
	;restauro RDI
	POP R13
	POP R12
	;en RAX esta el resultado de reservaMemoria que es lo mismo que queremos devolver, so, piola
	ret

;---------------------------------------------------------------------------------------

; ~ void nodo_concatenar(nodo_t **self, nodo_t *siguiente)
	;//asigno el siguiente al nodo *self
	;(*self)->siguiente=siguiente;
nodo_concatenar:
	;en RDI viene el doble puntero al nodo
	;en RSI viene el puntero al siguiente
	PUSH R12
	MOV R12, [RDI];primera desreferencia
	MOV [R12+offset_siguiente], RSI; asigno al siguiente el valor de RSI(siguiente)
	POP R12
	ret

;---------------------------------------------------------------------------------------

;nodo_t** nodo_ultimo(nodo_t **self_p){
;	nodo_t ** doublePtr=self_p;
;	while((*self_p)!=NULL){
;		doublePtr=self_p;
;		self_p=&((*self_p)->siguiente);
;	}
;	return doublePtr;
;}
nodo_ultimo:
	;en RDI viene el doble puntero al nodo inicial
	PUSH R12
	MOV RAX, RDI;inicializo RAX con el nodo del parametro(cantidato parcial)
	;itero hacia "adelante" hasta el final de la lista (Sig == NULL)	
ciclo:CMP qword [RDI], NULL; comparo el contenido de RDI (*RDI) con NULL
    JE finCiclo ;si es NULL me voy al final del ciclo
	MOV RAX, RDI;doublePtr = self_p - cambio el candidato parcial
	
	MOV R12, [RDI];desreferencio self_p(RDI) y lo guardo en R12
	;MOV R12, [R12 + offset_siguiente];ahora guardo en R12 el valor del campo siguiente del nodo apuntado
	;por RDI
	LEA RDI, [R12 + offset_siguiente];pongo la direccion efectiva (&) en RAX
	JMP ciclo
finCiclo:	
	;en RAX esta el ultimo candidato parcial a ultimo
	POP R12
	ret

;---------------------------------------------------------------------------------------
;nodo_t* nodo_acceder(nodo_t *self, uint posicion){	
;	uint j=0;
;	nodo_t *pointer=self;
;	while((self!=NULL) && (j<=posicion)){				
;		pointer=self;		
;		self=self->siguiente;		
;		j++;
;	}	
;	return pointer;
;
;	Suponiendo posiciones validad como precondicion lo de arriba ES ANALOGO A	
;
;	while(posicion>0){
;		self=self->siguiente;	
;		posicion--;
;	}
;	return self;
;}
nodo_acceder:
	;en RDI tengo el puntero inicial
	;en ESI tengo la posicion 0 based a la cual quiero llegar
cicloAcceder: CMP dword ESI, 0
	JE finCicloAcceder
	MOV RDI, [RDI + offset_siguiente];avanzo sobre la linkedlist	
	DEC ESI
	JMP cicloAcceder
finCicloAcceder:
	MOV RAX, RDI;guardo res en RAX
	ret
;---------------------------------------------------------------------------------------

;//Borra toda una columna de nodos
;void nodo_borrar_con_hijos(nodo_t *self){
;	while(self!=NULL){
;		nodo_t *nodoTarget = self;
;		self=self->hijo;		
;		if(nodoTarget->tipo==String){
;			free(nodoTarget->valor.s);
;		}
;		free(nodoTarget);
;	}
;}

nodo_borrar_con_hijos:
	;en RDI viene el pointer entry
	PUSH R12
	MOV R12, RDI; voy a usar R12 para avanzar por la lista y RDI para llamar a free por convencion C
cicloBorrarConHijos: CMP R12, NULL
	JE finCicloBorrarConHijos
	MOV RDI, R12;guardo la referencia para eliminar	
	MOV R12, [R12 + offset_hijo];avanzo la linkedlist		
	;call nodo_borrar;en RDI esta el puntero...
	PUSH R12;protejo los registros por la llamada a nodo_borrar
	call nodo_borrar
	POP R12
	JMP cicloBorrarConHijos
finCicloBorrarConHijos: 
	POP R12
	ret

;	if(nodoTarget->tipo==String){
;		free(nodoTarget->valor.s);
;	}
;	free(nodoTarget);
nodo_borrar:
	;en RDI esta el puntero a eliminar
	CMP dword [RDI + offset_tipo], ENUM_string;si es char* hay que liberar eso tambien
	JNE eliminarNodoHacer
	PUSH RDI
	MOV RDI, [RDI + offset_valor];cargo el char*
	call free
	POP RDI
eliminarNodoHacer: 
	sub rsp,8
	call free;en RDI ya estaba el puntero correcto...
	add rsp, 8
	ret

;---------------------------------------------------------------------------------------

; ~ void lista_borrar(lista *self)
;void lista_borrar(lista_colgante_t *self){
;	nodo_t *pointer=self->primero;		
;	while(pointer!=NULL){
;		nodo_t *target=pointer;
;		pointer=pointer->siguiente;
;		nodo_borrar_con_hijos(target);				
;	}
;	free(self);
;}
lista_borrar:
	PUSH R12
	PUSH R13
	;en RDI llega el puntero a la lista, no lo quiero, quiero el primer nodo en R12 para iterar
	MOV R13, RDI; me guardo la referencia a la lista para borrarla al final
	MOV R12, [RDI + offset_primero]
cicloListaBorrar:  	CMP qword R12, NULL
					JE finCicloListaBorrar
					MOV RDI, R12
					MOV R12, [R12 + offset_siguiente]
					;en RDI ya esta el puntero de entrada a la columna a borrar
					sub rsp, 8
					call nodo_borrar_con_hijos					
					add rsp, 8
					JMP cicloListaBorrar
finCicloListaBorrar:
	MOV RDI, R13;pongo en RDI el ptr a la lista
	sub rsp, 8
	call free;libero la lista
	add rsp, 8
	POP R13 
	POP R12
	ret
	
;---------------------------------------------------------------------------------------
	
;//Imprime entre [ y ] en el archivo stream el valor recibido.
;void valor_imprimir(tipo_elementos tipo, valor_elemento valor, FILE *fd){
;	switch(tipo){
;		case Integer:
;			fprintf(fd, "[ %i ]", valor.i);				
;		break;
;		case Double:
;			fprintf(fd, "[ %f ]", valor.d);				
;		break;
;		case String:
;			fprintf(fd, "[ %s ]", valor.s);
;		break;
;	}
;}

valor_imprimir:
	;en EDI viene el tipo de elemento
	; en RSI viene el valor
	; en RDX viene el file descriptor
	PUSH RAX; lo voy a usar para fprintf para la cantidad de parametros no enteros
	PUSH R12
	PUSH R13
		
	MOV R12, RSI;valor
	MOV R13, RDX;fd
	
	;----------
	;declaradas en section .data
	;formatoPrintfString: DB '[ %s ]', 0
	;formatoPrintfInteger: DB '[ %i ]', 0
	;formatoPrintfDouble: DB '[ %f ]', 0
caseImprimirInt:
			CMP dword EDI, ENUM_int
			JNE caseImprimirDouble
			;fprintf para entero
			MOV RAX, 0 ;cantidad de parametros no enteros
			;la pila esta alineada porque call + push RAX + R12 + R13
			;paso parametros por convencion C...
			MOV RDI, R13;fd
			MOV RSI, formatoPrintfInteger;formato
			MOV RDX, R12;valor
			call fprintf
			JMP finValorImprimir
caseImprimirDouble:			
			CMP dword EDI, ENUM_double
			JNE caseImprimirString
			;fprintf para double
			MOV RAX, 1 ;cantidad de parametros no enteros
			;la pila esta alineada porque call + push RAX + R12 + R13
			;paso parametros por convencion C...
			MOV RDI, R13;fd
			MOV RSI, formatoPrintfDouble;formato			
			MOVQ XMM0, R12
			call fprintf
			JMP finValorImprimir
caseImprimirString:			
			CMP dword EDI, ENUM_string
			JNE finValorImprimir
			;fprintf para char*
			MOV RAX, 0 ;cantidad de parametros no enteros
			;la pila esta alineada porque call + push RAX + R12 + R13
			;paso parametros por convencion C...
			MOV RDI, R13;fd
			MOV RSI, formatoPrintfString;formato
			MOV RDX, R12;valor
			call fprintf
			JMP finValorImprimir	
finValorImprimir:
	POP R13
	POP R12
	POP RAX
	ret

;---------------------------------------------------------------------------------------

; ~ void lista_imprimir(lista *self, char *archivo)
;void lista_imprimir(lista_colgante_t *self, char *archivo){
;	FILE* fd = fopen(archivo, "a");
;	if(fd==NULL){
;		fprintf(stderr, "\n\nERROR ABRIENDO EL ARCHIVO %s\n\n", archivo);
;	}else{		
;		printToStream(self, fd);
;		fclose(fd);
;	}	
;}
lista_imprimir:
	;en RDI viene el pointer a lista
	;en RSI viene el pointer al filepath
	;protego registros a usar por la funcion
	PUSH RAX
	PUSH R12
	PUSH R13
	;guardo argumentos en R12 y R13
	MOV R12, RDI;listaptr
	MOV R13, RSI;filepath char*
	;voy a hacer una llamada a fopen y en RAX me va a quedar el resultado
	;convencion C
	MOV RDI, R13
	MOV RSI, fileopenMode;constante 'a\0' en section .data
	;protejo registros de fopen
	call fopen;notar que la pila esta alineada por CALL y PUSHES
	;restauro registros protegidos de fopen
	;comparo RAX(el puntero del file descriptor con NULL)
	CMP RAX, NULL;si es null alto yao
	JE altoYaoImprimirLista;si es null pinta bardo asi que nos vamos
	;convencion C para llamar a printToStream
	MOV RDI, R12
	MOV RSI, RAX
	;protejo RAX de printToStream que tiene el fd
	;LLAMO 2 VECES A PUSH PARA ALINEAR LA PILA
	PUSH RAX
	PUSH RAX
	CALL printToStream
	;restauro RAX(que tiene el fd)
	POP RAX
	POP RAX
	;SON 2 POP POR LA ALINEACION DE LA PILA
	;--fclose
	MOV RDI, RAX
	CALL fclose
	JMP finImprimirLista
altoYaoImprimirLista:
	;error abriendo el archivo
	MOV RDI, fileopenError
	MOV RSI, R13
	MOV RAX, 0
	call printf
finImprimirLista:
	POP RAX
	POP R13
	POP R12
	ret

;---------------------------------------------------------------------------------------------
;void nodo_imprimir_columna(nodo_t *self, FILE *fd){
;	fprintf(fd, "{ ");
;	while(self!=NULL){						
;		valor_imprimir(self->tipo, self->valor, fd);		
;		self=self->hijo;
;	}
;	fprintf(fd, " }");
;}

nodo_imprimir_columna:
	;en RDI viene el self*
	;en RSI viene el fd*
	PUSH RAX;a
	PUSH R12;d
	PUSH R13;a
	;guardo parametros en R12 y R13
	MOV R12, RDI;self
	MOV R13, RSI;fd
	;open bracket call llamada a fprintf
	MOV RDI, R13
	MOV RSI, formatoOpenBracket
	MOV RAX, 0
	;protejo R12,R13 de fprintf
	PUSH R12;d
	PUSH R13;a
	call fprintf
	;restauro R12,R13
	POP R13
	POP R12
	
cicloImprimirColumna: 	CMP R12, NULL
						JE finCicloImprimirColumna
						MOV RDI, [R12+offset_tipo]
						MOV RSI, [R12+offset_valor]
						MOV RDX, R13
						call valor_imprimir
						MOV R12, [R12+offset_hijo]
						JMP cicloImprimirColumna
						
finCicloImprimirColumna:
	;close bracket call llamada a fprintf
	MOV RDI, R13
	MOV RSI, formatoCloseBracket
	MOV RAX, 0
	;protejo R12,R13 de fprintf
	PUSH R12
	PUSH R13
	call fprintf
	;restauro R12,R13
	POP R13
	POP R12
	
	POP R13
	POP R12
	POP RAX
	ret

;---------------------------------------------------------------------------------------------

;//Idem anterior pero para los descendientes, usa puntero a nodo directamente.
;nodo_t *nodo_ultimo_hijo(nodo_t *self){
;	nodo_t *ptr=self;		
;	while(self!=NULL){
;		ptr=self;
;		self=self->hijo;
;	}
;	return ptr;
;}
nodo_ultimo_hijo:
	;en RDI viene el pointer entry
	SUB RSP, 8	
cicloUltimoHijo:	CMP qword RDI, NULL
					JE finCicloUltimoHijo
					MOV RAX, RDI;actualizo el candidato parcial
					MOV RDI, [RDI+offset_hijo]
					JMP cicloUltimoHijo
finCicloUltimoHijo:
	ADD RSP, 8
	ret

;---------------------------------------------------------------------------------------------
; ~ void lista_concatenar(lista *self, nodo_t *nodo)
;void lista_concatenar(lista_colgante_t *self, nodo_t *siguiente){
;	nodo_t ** nodoDblPtr = nodo_ultimo(&self->primero);	
;	if((*nodoDblPtr)==NULL){
;		//la lista estaba vacia...lo asigno como primero
;		self->primero=siguiente;		
;	}else{
;		//concateno atras del nodo
;		nodo_concatenar(nodoDblPtr, siguiente);
;	}
;}
lista_concatenar:	
	;en RDI viene el self pointer a la lista
	;en RSI viene el pointer al nodo siguiente
	PUSH R12;a
	PUSH R13;d
	
	MOV R12, RDI;lista
	MOV R13, RSI;siguiente
	
	;convencion C para nodo_ultimo
	LEA RDI, [R12+offset_primero];paso la referencia (&)
	CALL nodo_ultimo
	;en RAX tengo el puntero al ultimo nodo de la lista
	;hay 2 casos
	;[RAX] NULL
	CMP qword [RAX], NULL
	JNE concatenarAtras 
	;la lista esta vacia, lo asigno como primero
	MOV [R12 + offset_primero], R13
	JMP finListaConcatenar
concatenarAtras:
	;llamo por convencion C a nodo_concatenar
	MOV RDI, RAX
	MOV RSI, R13
	CALL nodo_concatenar
	JMP finListaConcatenar
finListaConcatenar:
	POP R13
	POP R12
	ret
;---------------------------------------------------------------------------------------------

; ~ void lista_colgar_descendiente(lista *self, uint posicion, nodo_t *nodo)
;void lista_colgar_descendiente(lista_colgante_t *self, uint posicion, nodo_t *hijo){
;	//dos casos
;	if(self->primero==NULL){
;		//Caso 1:Lista vacia => concateno al final en vez de colgar como hijo
;		lista_concatenar(self, hijo);
;	}else{
;		//Caso 2: cuelgo como hijo
;		//obtengo el iesimo
;		nodo_t * iesimoNodo = nodo_acceder(self->primero, posicion);
;		//obtengo el ultimo de la columna
;		nodo_t * ultimoDeLaColumna = nodo_ultimo_hijo(iesimoNodo);
;		//lo cuelgo como hijo de ese ultimo
;		ultimoDeLaColumna->hijo=hijo;	
;	}
;}
lista_colgar_descendiente:
	;en RDI viene el * a lista
	;en ESI viene la posicion
	;en RDX viene el hijo*
	PUSH R12
	PUSH R13
	PUSH R14

	;copio parametros
	XOR R13, R13;limpio R13
	MOV R12, RDI;lista*
	MOV R13d, ESI;posicion
	MOV R14, RDX;hijo*

	;pregunto si la lista esta vacia
	CMP qword [R12+offset_primero], NULL
	JE listaVaciaColgarDescendiente
;	listaNoVaciaColgarDescendiente:
	;llamo a nodo_acceder por convencion C
	MOV RDI, [R12+offset_primero]
	MOV ESI, R13d
	CALL nodo_acceder
	;en RAX tengo el puntero al iesimo Nodo
	;llamo por convencion C a nodo_ultimo_hijo
	MOV RDI, RAX
	CALL nodo_ultimo_hijo
	;en RAX tengo el ultimo hijo
	MOV [RAX+offset_hijo], R14;concateno hijo nuevo
	JMP finColgarDescendiente	
listaVaciaColgarDescendiente:
	;llamo por convencion C a lista_concatenar
	MOV RDI, R12
	MOV RSI, R14
	CALL lista_concatenar
	JMP finColgarDescendiente
finColgarDescendiente:
	POP R12
	POP R13
	POP R12

	ret
;---------------------------------------------------------------------------------------------

;int min(int a, int b){
;	return (a<b)? a : b;
;}
min:
		;en EDI viene a
		;en ESI viene b
		MOV qword RAX, 0;limpio rax
		CMP dword EDI, ESI
		JA minimoA
minimoB:
		MOV EAX, ESI
		JMP finMin
minimoA:
		MOV EAX, EDI
		JMP finMin
finMin:
	ret
;---------------------------------------------------------------------------------------------

;void printToStream(lista_colgante_t *self, FILE* fd){	
;	if(self==NULL){
;		fprintf(fd, "<vacia>\n");
;	}else{
;		while(self!=NULL){			
;			nodo_t *hijosPtr = self;
;			nodo_imprimir_columna(hijosPtr, fd);
;			if(self->siguiente!=NULL){
;				fprintf(fd, " ");
;			}			
;			self=self->siguiente;	
;		}
;		fprintf(fd, "\n");
;	}	
;}

printToStream:
	;en RDI viene lista*
	;en RSI viene fd*
	PUSH R12
	PUSH R13
	PUSH RAX
	
	MOV R12, [RDI+offset_primero]; primero de la lista
	MOV R13, RSI;fd
	
;	;pregunto si lista es null
	CMP qword R12, NULL
	JE printSelfNull
cicloPrintToStream:		CMP qword R12, NULL
						JE finCicloPrintToStream
;						llamamos a nodo_imprimir_columna por convencion C
						MOV RDI, R12;self
						MOV RSI, R13;fd
;						PUSH R12
;						PUSH R12
						CALL nodo_imprimir_columna
;						POP R12
;						POP R12
						CMP qword [R12+offset_siguiente], NULL
						JE avanzarCicloPts						
						MOV RAX, 0
						MOV RDI, R13
						MOV RSI, constanteEspacio;constante section .data
;						PUSH R12
;						PUSH R12
						CALL fprintf
;						POP R12
;						POP R12
avanzarCicloPts:		MOV R12, [R12+offset_siguiente]
						JMP cicloPrintToStream
finCicloPrintToStream:	
	;dejar ultimo enter
	MOV RAX, 0
	MOV RDI, R13;fd
	MOV RSI, constanteEnter;constante section .data
	CALL fprintf
	JMP finPrintToStream
printSelfNull:
	;llamada a fprintf(tenemos la pila alineada por CALL y PUSHES
	MOV RAX, 0;cantidad de argumentos no enteros
	MOV RDI, R13;fd
	MOV RSI, emptyListLegend;constante en section .data
	CALL fprintf
finPrintToStream:
	POP RAX
	POP R13
	POP R12
	ret

;---------------------------------------------------------------------------------------------
;void valor_elemento_liberar_recursos(valor_elemento valor, tipo_elementos tipo){
;	//si es String hay que liberar el char* del union
;	if(tipo == String){
;		free(valor.s);
;	}
;}
valor_elemento_liberar_recursos:
	;en RDI viene valor
	;en ESI viene tipo
	CMP dword ESI, ENUM_string
	JNE finVELR
	SUB RSP, 8;alinear pila
	;en RDI ya esta el puntero
	call free
	ADD RSP, 8
finVELR:
	ret

;---------------------------------------------------------------------------------------------
;valor_elemento valor_hacer_copia_profunda(valor_elemento valor, tipo_elementos tipo){
;	//realizar una deep copy en caso de no ser string
;	valor_elemento res;
;	if(tipo != String){
;		//son tipos primitivos, esta todo mas que piola
;		res = valor;
;	}else{
;		//es char*, es decir String
;		//si asigno puntero a puntero tengo una shallow copy del valor		
;		res.s = (char *) malloc(strlen(valor.s) + 1);
;		strcpy(res.s, valor.s);	
;	}
;	return res;
;}
valor_hacer_copia_profunda:
	;en RDI viene valor
	;en ESI viene tipo
	CMP dword ESI, ENUM_string
	JE copiaProfundaString
	MOV RAX, RDI;devuelvo el valor tal cual
	JMP finCopiaProfunda
copiaProfundaString:
	;calculo tamanio del valor
	PUSH R12;alinear pila
	MOV R12, RDI;copio valor
	CALL strlen
	;en EAX tengo el largo,
	INC EAX;+1 para el \0
	MOV RDI, 0
	MOV EDI, EAX;convencion C para malloc
	CALL malloc
	;en RAX tengo el puntero nuevo
	;convencion C para strcpy
	MOV RDI, RAX;dst
	MOV RSI, R12;src
	CALL strcpy 
	POP R12
finCopiaProfunda:
	ret

;---------------------------------------------------------------------------------------------
;boolean tiene_numeros(nodo_t *n){	
;	char* charIndex = n->valor.s;
;	int numCount = 0;		
;	while((*charIndex)!='\0'){
;		//ascii value in charIndex pointer has to be in range [48..57] to be a number
;		numCount += ( ( (*charIndex) >=48 ) && ( (*charIndex) <=57 ) );
;		charIndex+=1;//le sumo un byte al puntero, porque los chars ocupan un byte
;	}
;	return (numCount>0) ? True : False;
;}
tiene_numeros:
;Nota: lo quise hacer con JA y JG negando el rango pero no pude, asi que salio esta bestialidad con JE
	;uso RAX como numCount
	MOV RAX, False
	;en RDI viene el nodo
	MOV RDI, [RDI+offset_valor]; ahora tengo el valor(es una string), un char*
cicloTieneNumeros:	CMP byte [RDI], NULL;NULL==0=='\0'
					JE finCicloTieneNumeros
					CMP byte [RDI], 48
					JE incSumaTNumeros	
					CMP byte [RDI], 49
					JE incSumaTNumeros
					CMP byte [RDI], 50
					JE incSumaTNumeros
					CMP byte [RDI], 51
					JE incSumaTNumeros
					CMP byte [RDI], 52
					JE incSumaTNumeros
					CMP byte [RDI], 53
					JE incSumaTNumeros
					CMP byte [RDI], 54
					JE incSumaTNumeros
					CMP byte [RDI], 55
					JE incSumaTNumeros
					CMP byte [RDI], 56
					JE incSumaTNumeros
					CMP byte [RDI], 57
					JE incSumaTNumeros
					JMP incCicloTNumeros
incSumaTNumeros:	INC RAX;incremento numCount
incCicloTNumeros:	INC RDI;++al puntero a chars,notar que sizeOf(char)==1
					JMP cicloTieneNumeros
finCicloTieneNumeros:
	CMP RAX, False	
	JE finTieneNumeros
	MOV RAX, True
finTieneNumeros:
	ret

;------------------------------------------------------------------------------------------

;valor_elemento raiz_de_la_suma(valor_elemento valorA, valor_elemento valorB){
;	valor_elemento valor;
;	valor.d = sqrt(valorA.d+valorB.d);
;	return valor;
;}
raiz_de_la_suma:
;	;en RDI viene valorA
;	;en RSI viene valorB
	MOVQ XMM0, RDI
	MOVQ XMM1, RSI
	ADDSD XMM0, XMM1
	;call sqrt
	;en XMM0 deberia estar el resu
	;hay que devolverlo por RAX porque como es union fuck logic con la convencion C
	;MOVQ XMM0, RAX
	;MOVQ XMM1, RAX
	SQRTSD XMM0, XMM0
	;MOVQ RAX, XMM0
	;MOVQ XMM0, 123
	MOVQ RAX, XMM0
	ret

;------------------------------------------------------------------------------------------

;valor_elemento raiz_cuadrada_del_producto(valor_elemento valorA, valor_elemento valorB){	
;	valor_elemento valor;
;	int res = valorA.i*valorB.i;
;	valor.i = (res>0) ? (int) sqrt(res) : 0;
;	return valor;
;}
raiz_cuadrada_del_producto:
	;en EDI viene valorA (int) 4 bytes!
	;en ESI viene valorB (int) 4 bytes!
	IMUL EDI, ESI
	CMP EDI, 0
	JG sacarRaizDelProducto
	MOV RAX, 0
	JMP finRaizCuadradaDelProducto
sacarRaizDelProducto:
	CVTSI2SD XMM0, EDI	
	SQRTSD XMM0, XMM0
	MOV RAX, 0
	CVTTSD2SI EAX, XMM0
finRaizCuadradaDelProducto:	
	ret

;---------------------------------------------------------------------------------------------
;en EAX viene el valor
;en EAX vuelve EAX / 10
;en EDX vuelve EAX % 10
modulo10:
	PUSH RCX
	
	MOV EDX, 0;limpio EDX
	MOV ECX, 10;pongo divisor
	DIV ECX
	
	POP RCX
	ret

;---------------------------------------------------------------------------------------------

;boolean tiene_ceros_en_decimal(nodo_t *n){
;	int value = abs(n->valor.i);
;	if(value==0) return False;	
;	int zeroCount=0;
;	while(value>0){
;		int remainder = value%10;
;		boolean isZero = (remainder==0);
;		if(isZero) 
;			zeroCount ++
;		value=(int) (value/10);
;	}	
;	return (zeroCount>0) ? True : False;
;}
tiene_ceros_en_decimal:
	PUSH RDI;valor
	PUSH R12;zerocount		
	PUSH RDX;auxiliar div
	MOV RDX, 0;inicializo RDX
	MOV R12, 0;uso RAX como zeroCount
	;en RDI viene el nodo
	MOV EDI, [RDI+offset_valor];en EDI tengo el valor, es entero asi que son 4 bytes
	CMP EDI, 0
	JE yaoDaCeroFalse;si valor==0 da false(hardcode tests catedra)			
		
	;HALLAR EL VALOR ABSOLUTO!!	
	TEST EDI, EDI; testeo negatividad de EDI
	JNS tengoAbs ; Si esta apagado el bit de signo, es positivo y salto el neg
	NEG EDI; change sign
tengoAbs:
	; tengo el abs en RDI	
	;pongo valor en EAX
	MOV EAX, EDI
cicloTieneCeros:	CMP EAX, 0
					JE finCicloTieneCeros	
	;en EAX viene el valor	
	CALL modulo10
	;en EAX vuelve EAX / 10
	;en EDX vuelve EAX % 10	
	CMP EDX, 0
	JNE seguirTieneCeros
	INC R12
seguirTieneCeros:
	JMP cicloTieneCeros
finCicloTieneCeros:
	CMP R12, False;comparo con False
	JE finTieneCeros ; si es False sale, sino pone True(cambia contador por un uno)
	MOV RAX, True
	JMP finTieneCeros
yaoDaCeroFalse:
	MOV RAX, False
finTieneCeros:
	POP RDX	
	POP R12
	POP RDI
	ret

;---------------------------------------------------------------------------------------------
;boolean parte_decimal_mayor_que_un_medio(nodo_t *n){
;	double x=fabs(n->valor.d);			
;	return ( (x - floor(x)) > 0.5) ? True : False;
;}
parte_decimal_mayor_que_un_medio:
	PUSH R12
	MOV RAX, [RDI + offset_valor];copio el valor de la lista a un registro
	
	;compare to nan hack, segun los tests de la catedra debe dar falso en caso de ser nan
	MOV R12, RAX
	SHR R12, 32
	CMP R12d, 0xFFF80000
	JE seRedondeaHaciaAbajo

;aca vale que no es nan
	
	;absolute value hack para puntos flotantes en IEEE, limpiamos el bit de signo	
	SHL RAX, 1
	SHR RAX, 1	
		
;copio el valor a los registros multimedia
	movq XMM1, RAX
	movq XMM0, RAX	
	XOR RAX, RAX; limpio RAX
	;convert double floating point to 32 bit integer by truncation
	;alias casteo a int
	CVTTSD2SI EAX, XMM1
	;converto 32 bit integer to ieee754 double precision floating point
	;alias casteo a double
	CVTSI2SD XMM1, EAX
	;aca tengo la parte entera luego del doble casteo
	;resto x-floor(x) y me queda la parte decimal en XMM0
	SUBSD XMM0, XMM1
	;hago una comparacion de tipo 2 de puntos flotantes
	CMPSD XMM0, [oneHalf], 2
	MOVQ RAX, XMM0; muevo el resultado de la comparacion que se guarda en XMM0 en RAX
	CMP RAX, 0x00; comparo con una mascara de false (0x00)
	JE seRedondeaHaciaArriba
	;si no hace el salto, se redondea hacia abajo
	JMP seRedondeaHaciaAbajo
seRedondeaHaciaArriba:
	MOV RAX, True
	JMP finPDMUM
seRedondeaHaciaAbajo:
	MOV RAX, False
finPDMUM:
	POP R12
	ret
	
	
; ~ void lista_filtrar(lista *self, nodo_bool_method method)
;void lista_filtrar(lista_colgante_t *self, nodo_bool_method test_method){
;	if(self->primero != NULL){
;		nodo_t ** nodo = &(self->primero);
;		while((*nodo)!=NULL){
;			if(test_method(*nodo)){
;				nodo_t *toDelete = *nodo;
;				*nodo=(*nodo)->siguiente;
;				nodo_borrar_con_hijos(toDelete);
;			}else{
;				nodo=&((*nodo)->siguiente);
;			}
;		}
;	}
;}
lista_filtrar:
	PUSH R12;nodo**
	PUSH R13;auxiliar
	PUSH R14;test_method
	PUSH R15;auxiliar2
	;en RDI viene self*
	;en RSI viene test_method
	CMP qword [RDI+offset_primero], NULL
	JE finFiltrar
	MOV R14, RSI;test_method load
	LEA R12, [RDI+offset_primero]
cicloFiltrar:	CMP qword [R12], NULL
				JE finCicloFiltrar				
				
				MOV RDI, [R12]
				PUSH R12
				CALL R14;test_method
				POP R12	
				CMP RAX, False				
				JE incrCicloFiltrar
				;saltear nodo
				MOV R13, [R12];toDelete
				MOV R15, [R13+offset_siguiente]
				MOV [R12], R15
				;fin saltear nodo
				
				;borrar columna nodo toDelete
				MOV RDI, R13
				CALL nodo_borrar_con_hijos
				;fin borrar columna
				JMP cicloFiltrar
incrCicloFiltrar:
				MOV R13, [R12]				
				LEA R12, [R13+offset_siguiente]
				JMP cicloFiltrar
finCicloFiltrar:

finFiltrar:
	POP R15
	POP R14
	POP R13
	POP R12
	ret

;---------------------------------------------------------------------------------------------

;; ~ lista* lista_colapsar(lista *self, lista_bool_method test_method, valor_elemento_method value_method)
;void lista_colapsar(lista_colgante_t *self, nodo_bool_method test_method, nodo_value_method join_method){
;	//verifico que no haga bumba bumba raka taka taka bum el &(self->primero)
;	if(self->primero != NULL){
;		
;		//me guardo la referencia doble para poder cambiar el puntero que me apunta
;		//siendo el actual si tengo que hacer estas cosas
;		nodo_t **current = &(self->primero);		
;		//itero con el pointer
;		while(*current != NULL){
;			//testeo el nodo, notar que como es doble * va con desreferencia
;			if(test_method(*current) == True){	
;				//si entro aca es porque el nodo cumplio la condicion de test_method
;				//entonces decimos "let the game begin" y colapsamos la columna
;				nodo_colapsar_c(current, join_method);
;			}
;			//avanzamos
;			current = &((*current)->siguiente);			
;		}
;
;
;	}
;}
lista_colapsar:
	;en RDI viene lista*
	;en RSI test_method
	;en RDX join_method
	PUSH R12;nodo**
	PUSH R13;test_method
	PUSH R14;join_method
	PUSH R15;auxiliar
	PUSH RBP
	CMP qword [RDI+offset_primero], NULL
	JE finColapsar		
	LEA R12, [RDI+offset_primero]
	MOV R13, RSI;test_method copy
	MOV R14, RDX;join_method copy
cicloColapsar:	CMP qword [R12], NULL
				JE finCicloColapsar								
				MOV RDI, [R12]
				CALL R13;test_method
				CMP RAX, False
				JE incremento_puntero			
				
				;nodo_colapsar branch
				MOV RDI, R12;nodo**
				MOV RSI, R14;join_method				
				CALL nodo_colapsar
				;end nodo colapsar branch
incremento_puntero:
			MOV R15, [R12]
			LEA R12, [R15+offset_siguiente]
			JMP cicloColapsar
finCicloColapsar:
finColapsar:
	POP RBP
	POP R15
	POP R14
	POP R13
	POP R12
	ret
;------------------------------------------------------------------------------------------

;//Recibe un puntero a puntero a un nodo base y se encarga de colapsar todos sus descendientes.
;void nodo_colapsar(nodo_t **self_pointer, nodo_value_method join_method){
;	nodo_t *nodo = *self_pointer;
;	valor_elemento valor = valor_hacer_copia_profunda(nodo->valor, nodo->tipo);
;	nodo_t *nuevo = nodo_crear(nodo->tipo, valor);
;	nodo_t *hijo = nodo->hijo;
;	while(hijo != NULL){
;		valor_elemento resultado = join_method(nuevo->valor, hijo->valor);
;		valor_elemento_liberar_recursos(nuevo->valor, nodo->tipo);
;		nuevo->valor = resultado;
;		hijo = hijo->hijo;
;	}
;	nuevo->siguiente = nodo->siguiente;
;	*self_pointer = nuevo;
;	nodo_borrar_con_hijos(nodo);
;}
nodo_colapsar:
	;en RDI viene nodo(self_pointer)
	;en RSI join_method	
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 8;8 bytes variable resultado
	PUSH RBX;preservo RDI
	PUSH R12; nodo*
	PUSH R13; preserva RSI
	PUSH R14; nuevo
	PUSH R15; hijo	
	
	MOV R13, RSI;CUIDAR RSI pq no esta en convencion c como preservado!
	MOV RBX, RDI;CUIDAR RDI porque no esta en convencion C como preservado!!!
	MOV R12, [RBX];desreferencio doble puntero a nodo	
	;hago llamada a valor_hacer_copia_profunda
	MOV RDI, [R12+offset_valor];8 bytes, esta re piolini.
	MOV RSI, 0
	MOV ESI, [R12+offset_tipo]
	CALL valor_hacer_copia_profunda
	;en RAX tengo el nuevo valor
	;convencion C para crear nuevo nodo
	MOV RDI, 0
	MOV EDI, [R12+offset_tipo]
	MOV RSI, RAX;valor que viene de la llamada a valor_hacer_copia_profunda
	CALL nodo_crear
	;en RAX esta el nuevo nodo
	MOV R14, RAX;asigno a nuevo
	MOV R15, [R12+offset_hijo];asigno a hijo
	;declaro var local para resultado dentro del ciclo
	
cicloNodoCol: 	CMP R15, NULL
				JE finCicloNodoCol
				
				MOV RDI, [R14+offset_valor];nuevo->valor
				MOV RSI, [R15+offset_valor];hijo->valor
				CALL R13;llamo a join method
				;en RAX tengo el resultado
				MOV [RSP], RAX;variable local resultado
				;llamo a valor_elemento_liberar_recursos convencion C
				MOV RDI, [R14+offset_valor]
				MOV RSI, 0
				MOV ESI, [R15+offset_tipo]		
				CALL valor_elemento_liberar_recursos
				MOV RAX, [RSP];leo la variable local resultado
				MOV [R14+offset_valor], RAX;asigno a nuevo->valor
				;avanzo puntero
				MOV R15, [R15+offset_hijo]				
				JMP cicloNodoCol
finCicloNodoCol:
	;aca puedo usar los registros sin preocuparme ya termina la funcion
	;piso nodo hijo ya que no lo uso mas y es null
	;nuevo->siguiente = nodo->siguiente;
	;usando  como auxiliar R15 porque intel no permite memoria a memoria
	MOV R15, [R12+offset_siguiente]		
	MOV [R14+offset_siguiente], R15
	MOV [RBX], R14;	*self_pointer = nuevo;
	;convencion C para nodo_borrar_con_hijos
	MOV RDI, R12
	CALL nodo_borrar_con_hijos
finNodoCol:			
	
	POP R15
	POP R14
	POP R13
	POP R12
	POP RBX
	ADD RSP, 8
	POP RBP
	ret

;---------------------------------------------------------------------------------------------
;
;valor_elemento revolver_primeras_5(valor_elemento vA, valor_elemento vB){	
;	//creo copias de los punteros de los parametros
;	char *x = vA.s;
;	char *y = vB.s;
;	//se agrega uno mas para el caracter nulo \0 que indica la terminacion de la cadena
;	//declaro 3 variables auxiliares para utilizar como indices en el merge
;	int i = 0;//corresponde al indice de x
;	int j = 0;//corresponde al indice de y
;	int k = 0;//corresponde al indice de z
;	//declaro 3 variables como limites de iteracion
;	int kLimit = 10;
;	int iLimit = strlen(x);
;	int jLimit = strlen(y);	
;	//asigno espacio en heap para kLimit+1(\0) char*'s	
;	char *z = (char *) malloc(kLimit+1/* *sizeof(char*) da uno en la arquitectura*/);
;	
;	//inicializo la porcion de memoria iniciada con base en z y 11 bytes despues con \0
;	memset(z, '\0', kLimit+1);
;	//simulo turnos con un contador modular para saber que string concatenar y que indice mover
;	int modCount = 0;
;	//hago una variacion del merge para listas ordenadas, pero tomando modCount como criterio
;	while(k < kLimit && i < iLimit && j < jLimit){
;		//concateno el correspondiente
;		if(modCount == 0){
;			z[k] = x[i];
;			i++;
;		}else{
;			z[k] = y[j];			
;			j++;
;		}
;		//avanzo indice salida y cambio turno
;		k++;
;		modCount=(modCount+1)%2;
;	}
;	
;	//si quedo agotada la string a, completo hasta el final con b
;	if(i == iLimit){
;		while(k < kLimit && j < jLimit){
;			z[k] = y[j];
;			j++;
;			k++;
;		}
;	}
;	//si quedo agotada la string b, completo hasta el final con a
;	if(j == jLimit){
;		while(k < kLimit && i < iLimit){
;			z[k] = x[i];
;			k++;
;			i++;
;		}
;	}
;	//wrappeo en valor_resultado el merge
;	valor_elemento resultado;
;	resultado.s = z;
;	return resultado;
;}
revolver_primeras_5:
	;los valor_elemento son de 8 bytes, so:
	;en RDI viene vA
	;en RSI viene vB
	PUSH RBP
	MOV RBP, RSP	
	PUSH RBX;auxiliar
	PUSH R12;vA
	PUSH R13;vB
	PUSH R14;char * salida
	PUSH R15;modCount
	PUSH R10;auxiliar
	PUSH R11;auxiliar
	SUB RSP, 8*6;8 bytes * 3 variables indices i,j,k + 3 variables indices iLimit, jLimit, kLimit
;	//creo copias de los punteros de los parametros
;	char *x = vA.s;
;	char *y = vB.s;
	MOV R12, RDI;vA
	MOV R13, RSI;vB
;	//se agrega uno mas para el caracter nulo \0 que indica la terminacion de la cadena
;	//declaro 3 variables auxiliares para utilizar como indices en el merge
;	int i = 0;//corresponde al indice de x
;	int j = 0;//corresponde al indice de y
;	int k = 0;//corresponde al indice de z
;	//declaro 3 variables como limites de iteracion
;	int kLimit = 10;
;	int iLimit = strlen(x);
;	int jLimit = strlen(y);	

;	inicializo las 6 var de pila en 0
	MOV qword [RSP + 0], 0;RSP+0 => i
	MOV qword [RSP + 8], 0;RSP+0 => j
	MOV qword [RSP + 16], 0;RSP+0 => k

	;limits con strlen
	MOV RDI, R12
	call strlen
	MOV qword [RSP + 24], RAX;RSP+0 => iLimit
	MOV RDI, R13
	call strlen
	MOV qword [RSP + 32], RAX;RSP+0 => jLimit
	;kLimit = 10
	MOV qword [RSP + 40], 10;RSP+0 => kLimit=10

;	//asigno espacio en heap para kLimit+1(\0) char*'s	
;	char *z = (char *) malloc(kLimit+1/* *sizeof(char*) da uno en la arquitectura*/);

	MOV RBX, [RSP+40];leo kLimit
	INC RBX;kLimit+1
	MOV RDI, RBX;kLimit+1
	SUB RSP, 8;alineamiento
	call malloc
	ADD RSP, 8;alineamiento
	MOV R14, RAX;asigno el puntero a salida
	
;	//inicializo la porcion de memoria iniciada con base en z y 11 bytes despues con \0
;	memset(z, '\0', kLimit+1);
;	//simulo turnos con un contador modular para saber que string concatenar y que indice mover
;	int modCount = 0;
;	//hago una variacion del merge para listas ordenadas, pero tomando modCount como criterio

	MOV RDI, R14;ptr a salida
	MOV RSI, 0;/0
	MOV RBX, [RSP+40];leo kLimit
	INC RBX;kLimit+1
	MOV RDX, RBX;kLimit+1
	SUB RSP, 8;alineamiento
	call memset
	ADD RSP, 8;alineamiento
	MOV R15, 0;modCount=0;
	
;	//hago una variacion del merge para listas ordenadas, pero tomando modCount como criterio
;	while(k < kLimit && i < iLimit && j < jLimit){
;		//concateno el correspondiente
;		if(modCount == 0){
;			z[k] = x[i];
;			i++;
;		}else{
;			z[k] = y[j];			
;			j++;
;		}
;		//avanzo indice salida y cambio turno
;		k++;
;		modCount=(modCount+1)%2;
;	}

cicloRevolver:	MOV RBX, [RSP+0];leo i
				CMP RBX, [RSP+24]; i vs iLimit
				JGE finCicloRevolver
				MOV RBX, [RSP+8];leo j
				CMP RBX, [RSP+32]; j vs jLimit
				JGE finCicloRevolver
				MOV RBX, [RSP+16];leo k
				CMP RBX, [RSP+40]; k vs kLimit
				JGE finCicloRevolver				
				
				CMP R15, 0;me fijo el turno...
				JNE turno1
				;vale turno0...
				MOV R11, [RSP+0];leo i
				MOV R10b, [R12+R11];leo vA.s[i]
				MOV RBX, [RSP+16];leo k
				MOV [R14+RBX], R10b; salida[k]=vA.s[i]
				INC qword [RSP+0];i++
				JMP avanzarRev
turno1:				
				MOV R11, [RSP+8];leo j
				MOV R10b, [R13+R11];leo vB.s[j]
				MOV RBX, [RSP+16];leo k
				MOV byte [R14+RBX], R10b; salida[k]=vB.s[j]
				INC qword [RSP+8];j++
				JMP avanzarRev
				
avanzarRev:		INC qword [RSP+16];k++
				CMP R15, 0
				JE ponerTurno1
				MOV R15, 0
				JMP cicloRevolver
ponerTurno1:
				MOV R15, 1
				JMP cicloRevolver
finCicloRevolver:

;	//si quedo agotada la string a, completo hasta el final con b
;	if(i == iLimit){
;		while(k < kLimit && j < jLimit){
;			z[k] = y[j];
;			j++;
;			k++;
;		}
;	}
;	//si quedo agotada la string b, completo hasta el final con a
;	if(j == jLimit){
;		while(k < kLimit && i < iLimit){
;			z[k] = x[i];
;			k++;
;			i++;
;		}
;	}

				MOV RBX, [RSP+0];leo i
				CMP RBX, [RSP+24]; i vs iLimit
				JNE agotamosA
				
cicloAgotarA:	MOV RBX, [RSP+8];leo j
				CMP RBX, [RSP+32]; j vs jLimit
				JGE finCicloAgotarA
				MOV RBX, [RSP+16];leo k
				CMP RBX, [RSP+40]; k vs kLimit
				JGE finCicloAgotarA	
				
				MOV R11, [RSP+8];leo j
				MOV R10b, [R13+R11];leo vB.s[j]
				MOV RBX, [RSP+16];leo k
				MOV byte [R14+RBX], R10b; salida[k]=vB.s[j]
				
								
				INC qword [RSP+8];j++
				INC qword [RSP+16];k++
				JMP cicloAgotarA
finCicloAgotarA:
				
agotamosA:		


				MOV RBX, [RSP+8];leo j
				CMP RBX, [RSP+32]; j vs jLimit
				JNE finRevolver
				
cicloAgotarB:	MOV RBX, [RSP+0];leo i
				CMP RBX, [RSP+24]; i vs iLimit
				JGE finCicloAgotarB
				MOV RBX, [RSP+16];leo k
				CMP RBX, [RSP+40]; k vs kLimit
				JGE finCicloAgotarB
				
				MOV R11, [RSP+0];leo i
				MOV R10b, [R12+R11];leo vA.s[i]
				MOV RBX, [RSP+16];leo k
				MOV [R14+RBX], R10b; salida[k]=vA.s[i]
				
								
				INC qword [RSP+0];i++
				INC qword [RSP+16];k++
				JMP cicloAgotarB
finCicloAgotarB:

finRevolver:
	MOV RAX, R14;return salida;
	ADD RSP, 8*6;desapilo las variables locales
	POP R11
	POP R10
	POP R15
	POP R14
	POP R13
	POP R12
	POP RBX	
	POP RBP
	ret	

;---------------------------------------------------------------------------------------------
