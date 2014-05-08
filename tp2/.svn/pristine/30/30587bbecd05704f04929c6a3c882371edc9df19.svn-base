global miniature_asm

section .data
ALIGN 16
MASK_SHIFT_PX_LEFT: DB 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,255

section .text
 %define PIXEL 3
 %define PX_BLUR_ITER 3
 %define PX_COPY_ITER 4
 %define NORMALIZAR_DIV_TRES 200 

; void miniature_asm(unsigned char *src,
;                unsigned char *dst,
;                int width,
;                int height,
;                float topPlane,
;                float bottomPlane,
;                int iters);
miniature_asm:


;--------------------------PARAMETROS----------------------------------
;RDI:	src
;RSI:	dst
;RDX:	width 	---se guarda en--->  R14
;RCX:	height
;XMM0:	topPlane
;XMM1:	bottomPlane
;R8:	iters
;----------------------------------------------------------------------


MOV R14, RDX ;RDX lo voy a usar en las funciones de multiplicar y dividir conjunto a RAX
MOVDQA	XMM3, [MASK_SHIFT_PX_LEFT]
;XMM1 -> LIBRE
;XMM2 -> LIBRE

		;----- cantidad_filas_top ---> R9 ----------------
	CVTSI2SS	XMM2, RCX
	MOVDQA		XMM4, XMM0
	MULSS		XMM4, XMM2
	CVTTSS2SI 	R9, XMM4

	;XMM2 = (float) height

	;----- cantidad_filas_bottom----> R10 ----------------
	MOVDQA		XMM4, XMM1
	MULSS		XMM4, XMM2
	CVTTSS2SI 	R13, XMM4
	MOV 		R10, RCX
	SUB			R10, R13





;----- iteracion --> R15 --------------------------
XOR	R15, R15
.cicloIteraciones:
	
	;resguardo valores para proxima iteracion
	PUSH	R9
	PUSH	R10
	PUSH	R15


	;Si me cargan una imagen cuyo numero width o height entre en 64bits
	;me como mi sombrero

	;----- cantidad_filas_top_evaluadas --> R9 ----
	MOV 	R11, R9
	IMUL	R11, R15
	MOV 	RAX, R11
	XOR		RDX, RDX
	IDIV	R8 ;DIVISION
	MOV 	R11, RAX
	SUB		R9, R11

	;truco para sega genesis
	CMP		RDX, 0 		;hubo resto en la division?
	JE 		.seguir
	DEC  	R9 	;redondeo de divisor a numero mas alto => menor filas	

	.seguir:
	;--- cantidad_filas_bottom_evaluadas --> R10 --
	MOV 	R11, R10
	IMUL	R11, R15
	XOR		RDX, RDX
	MOV 	RAX, R11
	IDIV	R8
	MOV 	R11, RAX
	SUB		R10, R11

	;--- ultima_fila_top_Plane --> R9 -------------

	;--- primera_fila_bottomPlane --> R10 ---------
	MOV 	R12, RCX
	SUB		R12, R10
	MOV		R10, R12

	;--- fila ----> RBX ---------------------------
	XOR		RBX, RBX
	.cicloFilas:

		;--- columna ----> R11 --------------------
		XOR		R11, R11
		.cicloColumnas:


		
		;-----------------Logica de borde-ultima fila------------------------
			;Evaluo el caso borde en el que no tenga 5 px a procesar, le resto a los ya procesados
			;los que no existen
			MOV 	R13, RCX
			DEC		R13
			CMP		R13, RBX
			JNE		.segundoCasoBorde
			;si es la ultima fila 
			MOV 	R13, R11	;columna actual
			MOV 	RAX, R14
			SUB		RAX, 6
			CMP		R13, RAX
			JL		.calcularIndice
			MOV 	R11, R14 	;me pongo en la ultima columna
			SUB		R11, 6 		;5 posiciones antes (-1 del width)
			JMP		.calcularIndice
		

		.segundoCasoBorde:
		;---Logica borde ultima fila filtrable--------------------------
		;Como analizo de a 3 los ultimos 2 o 1 pixel que analizo en la banda podrian
		;acarrearme a un Segmentation fault, con lo cual no esta bueno
			MOV 	R13, RCX
			SUB 	R13, 3
			CMP		R13, RBX
			JNE		.calcularIndice
			;si es la ultima fila filtrable
			;veo si es una columna de borde (o sea no filtrable)
			MOV 	R13, R11
			ADD 	R13, 2
			CMP 	R13, R14
			JGE 	.calcularIndice

			;veo si es una columna de las ultimas para procesar
			MOV 	R13, R11; columna actual
			ADD 	R13, 6;columna actual + pixelesANecesitar
			MOV 	RAX, R14
			CMP		R13, RAX;columna actual +5 == width
			JL		.calcularIndice
			MOV 	R11, R14 ; me pongo en la ultima columna
			SUB		R11, 6 ; 5 posiciones antes (-1 del width)
			JMP		.calcularIndice






			.calcularIndice:
			;--- indice_pixel ----> R12 -----------
			MOV		R13, R11	;columna
			IMUL 	R13, PIXEL 	;columna x PIXEL
			MOV		R12, RBX	;fila
			IMUL 	R12, R14	;fila x width
			IMUL	R12, PIXEL 	;fila x width x PIXEL
			ADD		R12, R13 	;fila x width x PIXEL + columna x PIXEL




			.limites:
			;------------ no_esBorde --------------
			CMP		RBX, 2		;fila > 2
			JLE		.dejarPixelIgual; &&
			MOV		R13, RCX	;height
			SUB		R13, 2		;height - 2
			CMP		RBX, R13	;fila < height - 2
			JGE		.dejarPixelIgual; &&
			CMP		R11, 1		;columna > 1
			JLE		.dejarPixelIgual; &&
			MOV		R13, R14	;width
			SUB		R13, 2		;width - 2
			CMP		R11, R13	;columna < width - 2
			JGE		.dejarPixelIgual

			;--------- estaDentroFrames-----------MODIF A FILTRAR
			CMP		RBX, R9		;fila <= ultima_fila_topPlane
			JLE		.filtrar	; ||
			CMP		RBX, R10	;fila >= primera_fila_bottomPlane
			JGE		.filtrar

			
;---------------------------ESTADO DE REGISTROS-------------------------------------
;RDI:		src
;RSI:		dst
;RCX:		height
;PILA 2:	cantidad_filas:Top
;PILA 1:	cantidad_filas:Bottom 
;PILA 0:	iteracion_actual
;R8:		iteraciones totales
;RBX:		fila actual
;R9:		ultima_fila_topPlane
;R10:		primera_fila_bottomPlane
;R11:		columna actual
;R12:		indice_pixel

;R13:		auxiliar
;RAX:		auxiliar
;RDX:		auxiliar
	
;R14:		width
;R15:		procesamiento batch
;XMM3:		MASK_SHIFT_PX_LEFT
;XMM resto: LIBRES
;----------------------------------------------------------------------------------


;-----------------------------------------------------------------------------------------------------
;----------------------------DEJAR PIXEL IGUAL--------------------------------------------------------
;-----------------------------------------------------------------------------------------------------



	
			.dejarPixelIgual:

			;---Continuo con la logica de borde----------
			MOV 	R13, RCX ; height
			DEC		R13			;height -1
			CMP		R13, RBX	; height == fila actual ?
			JNE		.seguirIgual;false
			MOV 	RAX, R14	;width
			SUB		RAX, 6 		;width -6 (width -1 porque empieza en 0 - 5 px)
			CMP 	RAX, R11	; width -6 == columna actual?
			JNE		.seguirIgual;false
			MOV 	R11, R14
			DEC 	R12
			SUB 	R11, 2		; finalizar fila
			MOV 	R15, 0 ;fuerzo a levantar



			.seguirIgual:
			;Evaluo si el pixel ya lo copie en el procesamiento batch

			CMP R15, PX_COPY_ITER ; la levantada anterior fue de procesamiento?
			JG 	.levantar
			CMP R15, 0 ;el pixel actual todavia no fue copiado?
			JE 	.levantar
			JMP .siguientePixel

			.levantar:
			MOV		R13, RDI		;src
			ADD		R13, R12		;src+indice_pixel
			MOVDQU 	XMM0, [R13]		;[indice_src+indice_pixel] aca estoy levantando 5px
			MOV 	R15, PX_COPY_ITER
			JMP		.escribir	

;-----------------------------------------------------------------------------------------------------
;----------------------------FILTRAR------------------------------------------------------------------
;-----------------------------------------------------------------------------------------------------


			.filtrar:


			;---Continuo con la logica de borde----------
			MOV 	R13, RCX
			SUB 	R13, 3
			CMP		R13, RBX
			JNE		.seguirFiltrar
			;si es la ultima fila filtrable
			;veo si es una columna de borde (o sea no filtrable)
			MOV 	R13, R11
			ADD 	R13, 2
			CMP 	R13, R14
			JGE 	.seguirFiltrar

			;veo si es una columna de las ultimas para procesar
			MOV 	R13, R11; columna actual
			ADD 	R13, 6;columna actual + pixelesANecesitar
			MOV 	RAX, R14
			CMP		R13, RAX;columna actual +5 == width
			JL		.seguirFiltrar
			MOV 	R11, R14 ; me pongo en la ultima columna
			SUB		R11, 3 	;el proximo que levante va a ser del borde
			MOV 	R15, 6 	;fuerzo a levantar el maximo
			JMP		.seguirFiltrar


			.seguirFiltrar:
			;Evaluo si el pixel ya lo copie en el procesamiento batch

			CMP 	R15, 6 ; la levantada anterior fue de copiado directo?
			JLE 	.levantarYProcesar
			JMP 	.siguientePixel

			

	

;-----------------------------------------------------------------------------------------------------
;----------------------------PROCESAR PIXEL SIMD------------------------------------------------------
;-----------------------------------------------------------------------------------------------------

		.levantarYProcesar:
;en XMM10, XMM11 y XMM12 voy a guardar los datos de los pixeles procesados
;en XMM13 voy a guardar los pixeles originales de la fila

		MOV		R13, RDI		;src
		ADD		R13, R12		;src+indice_pixel

		;-----------INDICE OFFSET 0 == RDX -------------------------- 
		MOV 	RDX, PIXEL 	; PIXEL
		IMUL	RDX, 2 		; PIXEL * 2
		MOV 	RAX, RDX	; PIXEL * 2
		IMUL	RAX, R14	; PIXEL * 2 * width
		ADD 	RAX, RDX	; PIXEL * 2 * width + PIXEL * 2
		MOV 	RDX, R13
		SUB		RDX, RAX	;indice - (PIXEL * 2 * width + PIXEL * 2)

		;registros de datos de pixeles
		XORPD	XMM10, XMM10
		XORPD	XMM11, XMM11
		XORPD	XMM12, XMM12
		XORPD	XMM15, XMM15 ; este va a ser mi registro null


		;levanto primer fila de los px cercanos

		MOVDQU 		XMM0, [RDX]		;5 primeros pixeles
		MOVDQA		XMM13, XMM0 	;resguardo valor
		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 ; de xmm1 solo me interesa el 1er word

		;Me genero el registro de multiplicacion en XMM14

		MOV 		EAX, 1
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		;uso el XMM15 que esta en 0 para replicar los valores
		PSHUFD		XMM14, XMM14, 0h



;---------------PRIMER FILA | PRIMER COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------


		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14 
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4; solo me interesaba el primer word, ahora solo el primer DW

		;los valores multiplicados terminan en este orden de registros : XMM0, XMM2, XMM1
		; ahora tengo que sumarlos al resultado final, como son los primeros valores, simplemente los voy a expandir a 
		; doubleword al final solo voy a necesitar 3 registros XMM para guardar estos datos

		;paso los valores finales a los registros donde voy a guardar los datos de los pixeles a escribir
		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2 ;esto esta bien...como el nene, el nene esta bien
		PADDD	XMM12, XMM1




;---------------PRIMER FILA | SEGUNDO COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------
		;Multiplico por el segundo coeficiente necesito shiftear para acomodar los pixeles que necesito

		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		MOVDQA		XMM0, XMM13


		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 ; de xmm1 solo me interesa el 1er word

		;Me genero el registro de multiplicacion en XMM14

		MOV 		EAX, 5
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		;uso el XMM15 que esta en 0 para replicar los valores
		PSHUFD		XMM14, XMM14, 0h

		
		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14 ; solo me interesaba el primer word, ahora solo el primer DW
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4

		;Ahora sumo con los valores acumulados

		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2
		PADDD	XMM12, XMM1


;---------------PRIMER FILA | TERCER COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------
		;Shifteo nuevamente

		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		MOVDQA		XMM0, XMM13


		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 ; de xmm1 solo me interesa el 1er word

		;Me genero el registro de multiplicacion en XMM14

		MOV 		EAX, 18
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		;uso el XMM15 que esta en 0 para replicar los valores
		PSHUFD		XMM14, XMM14, 0h

		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14 ; solo me interesaba el primer word, ahora solo el primer DW
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4
		;Ahora sumo con los valores acumulados

		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2
		PADDD	XMM12, XMM1


;---------------PRIMER FILA | CUARTO COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------
		;Ahora tengo que levantar el 2do set de pixeles para terminar de procesar la fila

		MOVDQU 		XMM0, [RDX + 2*PIXEL]	;5 primeros pixeles desfazados en 2 posiciones
		
		
		PSHUFB		XMM0, XMM3
		PSHUFB		XMM0, XMM3
		PSHUFB		XMM0, XMM3
		MOVDQA		XMM13, XMM0 ;resguardo valores originales (alineados)

		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 ; de xmm1 solo me interesa el 1er word

		;para usar estos valores tengo que alinearlos

		;Me genero el registro de multiplicacion en XMM14

		MOV 		EAX, 5
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		;uso el XMM15 que esta en 0 para replicar los valores
		PSHUFD		XMM14, XMM14, 0h


		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14 ; solo me interesaba el primer word, ahora solo el primer DW
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4
		;Ahora sumo con los valores acumulados

		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2
		PADDD	XMM12, XMM1

;---------------PRIMER FILA | QUINTO COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------
		;Shifteo nuevamente

		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		MOVDQA		XMM0, XMM13


		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 ; de xmm1 solo me interesa el 1er word

		;Me genero el registro de multiplicacion en XMM14

		MOV 		EAX, 1
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		;uso el XMM15 que esta en 0 para replicar los valores
		PSHUFD		XMM14, XMM14, 0h

		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14 ; solo me interesaba el primer word, ahora solo el primer DW
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4
		;Ahora sumo con los valores acumulados

		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2
		PADDD	XMM12, XMM1








;---------------SEGUNDA FILA-------------------------------------------------------
;----------------------------------------------------------------------------------


		XORPD	XMM15, XMM15 

		;levanto segunda fila de los px cercanos
		MOV 	R13, R14
		IMUL	R13, PIXEL 	;width * PIXEL

		MOVDQU 		XMM0, [RDX + R13]		
		MOVDQA		XMM13, XMM0 	
		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 

		;Me genero el registro de multiplicacion en XMM14

		MOV 		EAX, 5
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		PSHUFD		XMM14, XMM14, 0h

;---------------SEGUNDA FILA | PRIMER COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------


		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14 
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4

		
		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2 
		PADDD	XMM12, XMM1




;---------------SEGUNDA FILA | SEGUNDO COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------

		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		MOVDQA		XMM0, XMM13


		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 

		;Me genero el registro de multiplicacion en XMM14

		MOV 		EAX, 32
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		PSHUFD		XMM14, XMM14, 0h

		
		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14 
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4

		;Ahora sumo con los valores acumulados

		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2
		PADDD	XMM12, XMM1


;---------------SEGUNDA FILA | TERCER COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------
		;Shifteo nuevamente

		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		MOVDQA		XMM0, XMM13


		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 

		;Me genero el registro de multiplicacion en XMM14

		MOV 		EAX, 64
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		PSHUFD		XMM14, XMM14, 0h

		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14 
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4
		;Ahora sumo con los valores acumulados

		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2
		PADDD	XMM12, XMM1


;---------------SEGUNDA FILA | CUARTO COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------
		

		MOVDQU 		XMM0, [RDX + R13 + 6]	
		PSHUFB		XMM0, XMM3
		PSHUFB		XMM0, XMM3
		PSHUFB		XMM0, XMM3
		MOVDQA		XMM13, XMM0 

		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 

		MOV 		EAX, 32
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		PSHUFD		XMM14, XMM14, 0h

		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4
		;Ahora sumo con los valores acumulados

		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2
		PADDD	XMM12, XMM1

;---------------SEGUNDA FILA | QUINTO COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------
		;Shifteo nuevamente

		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		MOVDQA		XMM0, XMM13


		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 

		;Me genero el registro de multiplicacion en XMM14

		MOV 		EAX, 5
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		PSHUFD		XMM14, XMM14, 0h

		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14 
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4
		;Ahora sumo con los valores acumulados

		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2
		PADDD	XMM12, XMM1




;---------------TERCER FILA-------------------------------------------------------
;----------------------------------------------------------------------------------


		XORPD	XMM15, XMM15 

		;levanto tercer fila de los px cercanos
		MOV 	R13, R14
		IMUL	R13, PIXEL 	;width * PIXEL

		MOVDQU 		XMM0, [RDX + 2 * R13]		
		MOVDQA		XMM13, XMM0 	
		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 


;---------------TERCER FILA | PRIMER COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------

		;Me genero el registro de multiplicacion en XMM14

		MOV 		EAX, 18
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		PSHUFD		XMM14, XMM14, 0h

		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14 
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4

		
		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2 
		PADDD	XMM12, XMM1




;---------------TERCER FILA | SEGUNDO COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------

		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		MOVDQA		XMM0, XMM13


		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 

		;Me genero el registro de multiplicacion en XMM14

		MOV 		EAX, 64
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		PSHUFD		XMM14, XMM14, 0h

		
		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14 
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4

		;Ahora sumo con los valores acumulados

		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2
		PADDD	XMM12, XMM1


;---------------TERCER FILA | TERCER COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------
		;Shifteo nuevamente

		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		MOVDQA		XMM0, XMM13


		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 

		;Me genero el registro de multiplicacion en XMM14

		MOV 		EAX, 100
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		PSHUFD		XMM14, XMM14, 0h

		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14 
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4
		;Ahora sumo con los valores acumulados

		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2
		PADDD	XMM12, XMM1


;---------------TERCER FILA | CUARTO COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------
		

		MOVDQU 		XMM0, [RDX + 2 * R13 + 6]	
		PSHUFB		XMM0, XMM3
		PSHUFB		XMM0, XMM3
		PSHUFB		XMM0, XMM3
		MOVDQA		XMM13, XMM0 

		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 

		MOV 		EAX, 64
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		PSHUFD		XMM14, XMM14, 0h

		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4
		;Ahora sumo con los valores acumulados

		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2
		PADDD	XMM12, XMM1

;---------------TERCER FILA | QUINTO COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------
		;Shifteo nuevamente

		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		MOVDQA		XMM0, XMM13


		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 

		;Me genero el registro de multiplicacion en XMM14

		MOV 		EAX, 18
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		PSHUFD		XMM14, XMM14, 0h

		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14 
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4
		;Ahora sumo con los valores acumulados

		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2
		PADDD	XMM12, XMM1



;---------------CUARTA FILA-------------------------------------------------------
;----------------------------------------------------------------------------------


		XORPD	XMM15, XMM15 

		;levanto segunda fila de los px cercanos
		MOV 	R13, R14
		IMUL	R13, PIXEL 	;width * PIXEL
		IMUL	R13, 3 ;caso especial por no poder hacer lea con 3

		MOVDQU 		XMM0, [RDX + R13]		
		MOVDQA		XMM13, XMM0 	
		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 


;---------------CUARTA FILA | PRIMER COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------

		;Me genero el registro de multiplicacion en XMM14

		MOV 		EAX, 5
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		PSHUFD		XMM14, XMM14, 0h

		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14 
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4

		
		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2 
		PADDD	XMM12, XMM1




;---------------CUARTA FILA | SEGUNDO COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------

		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		MOVDQA		XMM0, XMM13


		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 

		;Me genero el registro de multiplicacion en XMM14

		MOV 		EAX, 32
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		PSHUFD		XMM14, XMM14, 0h

		
		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14 
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4

		;Ahora sumo con los valores acumulados

		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2
		PADDD	XMM12, XMM1


;---------------CUARTA FILA | TERCER COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------
		;Shifteo nuevamente

		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		MOVDQA		XMM0, XMM13


		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 

		;Me genero el registro de multiplicacion en XMM14

		MOV 		EAX, 64
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		PSHUFD		XMM14, XMM14, 0h

		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14 
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4
		;Ahora sumo con los valores acumulados

		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2
		PADDD	XMM12, XMM1


;---------------CUARTA FILA | CUARTO COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------
		

		MOVDQU 		XMM0, [RDX + R13 + 6]	
		PSHUFB		XMM0, XMM3
		PSHUFB		XMM0, XMM3
		PSHUFB		XMM0, XMM3
		MOVDQA		XMM13, XMM0 

		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 

		MOV 		EAX, 32
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		PSHUFD		XMM14, XMM14, 0h

		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4
		;Ahora sumo con los valores acumulados

		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2
		PADDD	XMM12, XMM1

;---------------CUARTA FILA | QUINTO COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------
		;Shifteo nuevamente

		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		MOVDQA		XMM0, XMM13


		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 

		;Me genero el registro de multiplicacion en XMM14

		MOV 		EAX, 5
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		PSHUFD		XMM14, XMM14, 0h

		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14 
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4
		;Ahora sumo con los valores acumulados

		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2
		PADDD	XMM12, XMM1




;---------------QUINTA FILA-------------------------------------------------------
;----------------------------------------------------------------------------------


		XORPD	XMM15, XMM15 

		;levanto segunda fila de los px cercanos
		MOV 	R13, R14
		IMUL	R13, PIXEL 	;width * PIXEL

		MOVDQU 		XMM0, [RDX + 4 * R13]		
		MOVDQA		XMM13, XMM0 	
		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 

		;Me genero el registro de multiplicacion en XMM14

		MOV 		EAX, 1
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		PSHUFD		XMM14, XMM14, 0h

;---------------QUINTA FILA | PRIMER COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------


		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14 
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4

		
		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2 
		PADDD	XMM12, XMM1




;---------------QUINTA FILA | SEGUNDO COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------

		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		MOVDQA		XMM0, XMM13


		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 

		;Me genero el registro de multiplicacion en XMM14

		MOV 		EAX, 5
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		PSHUFD		XMM14, XMM14, 0h

		
		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14 
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4

		;Ahora sumo con los valores acumulados

		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2
		PADDD	XMM12, XMM1


;---------------QUINTA FILA | TERCER COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------
		;Shifteo nuevamente

		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		MOVDQA		XMM0, XMM13


		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 

		;Me genero el registro de multiplicacion en XMM14

		MOV 		EAX, 18
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		PSHUFD		XMM14, XMM14, 0h

		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14 
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4
		;Ahora sumo con los valores acumulados

		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2
		PADDD	XMM12, XMM1


;---------------QUINTA FILA | CUARTO COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------
		

		MOVDQU 		XMM0, [RDX + 4 * R13 + 5]	
		PSHUFB		XMM0, XMM3
		PSHUFB		XMM0, XMM3
		PSHUFB		XMM0, XMM3

		PSHUFB		XMM0, XMM3
		
		MOVDQA		XMM13, XMM0 

		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 

		MOV 		EAX, 5
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		PSHUFD		XMM14, XMM14, 0h

		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4
		;Ahora sumo con los valores acumulados

		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2
		PADDD	XMM12, XMM1

;---------------QUINTA FILA | QUINTO COEFICIENTE--------------------------------
;-------------------------------------------------------------------------------
		;Shifteo nuevamente

		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		PSHUFB		XMM13, XMM3
		MOVDQA		XMM0, XMM13


		MOVDQA		XMM1, XMM0
		PUNPCKLBW	XMM0, XMM15
		PUNPCKHBW	XMM1, XMM15 

		;Me genero el registro de multiplicacion en XMM14

		MOV 		EAX, 1
		PINSRW		XMM14, EAX, 0
		PINSRW		XMM14, EAX, 1
		PSHUFD		XMM14, XMM14, 0h

		MOVDQA		XMM2, XMM0
		PMULLW		XMM0, XMM14
		PMULHW		XMM2, XMM14
		MOVDQA		XMM4, XMM0
		PUNPCKLWD	XMM0, XMM2
		PUNPCKHWD	XMM4, XMM2
		MOVDQA		XMM2, XMM4
		MOVDQA		XMM4, XMM1
		PMULLW		XMM1, XMM14 
       	PMULHW		XMM4, XMM14
       	PUNPCKLWD	XMM1, XMM4
		;Ahora sumo con los valores acumulados

		PADDD	XMM10, XMM0
		PADDD	XMM11, XMM2
		PADDD	XMM12, XMM1






;----------------------------REARMADO DE PIXELES----------------------------------
;---------------------------------------------------------------------------------



		;normalizo los valores
		CVTDQ2PS	XMM10, XMM10
		CVTDQ2PS	XMM11, XMM11
		CVTDQ2PS	XMM12, XMM12

		;obtengo el divisor
		MOV 		EAX, NORMALIZAR_DIV_TRES
		PINSRW		XMM14, EAX, 0
		XOR 		EAX, EAX
		PINSRW		XMM14, EAX, 1
		PSHUFD		XMM14, XMM14, 0h
		CVTDQ2PS	XMM14, XMM14

		MOV 		EAX, 3
		PINSRW		XMM15, EAX, 0
		XOR 		EAX, EAX
		PINSRW		XMM15, EAX, 1
		PSHUFD		XMM15, XMM15, 0h
		CVTDQ2PS	XMM15, XMM15

		MULPS 		XMM14, XMM15

		XORPD		XMM15, XMM15



		;Divido todos
		DIVPS	XMM10, XMM14
		DIVPS	XMM11, XMM14
		DIVPS	XMM12, XMM14

		;Los reconvierto en integers con truncacion
		CVTTPS2DQ	XMM10, XMM10
		CVTTPS2DQ	XMM11, XMM11
		

		;packeo los pixeles devuelta a 24 bits c/u 

		CVTTSS2SI	EAX, XMM12 ;aca es cuando creemos que hay un dios que vela por nosotros
		MOV 		EDX, 255
		XORPD		XMM12, XMM12
		PINSRW		XMM12, EDX, 0 ;aca copio el word pero al ser unsigned y < a 256 queda como byte ;)
		XORPD 		XMM15, XMM15
		PSHUFB		XMM12, XMM15 ; lleno xmm12 de unos
		XORPD 		XMM14, XMM14
		PINSRW 		XMM14, EDX, 4
		XORPD		XMM12, XMM14; ahora hago un hueco de ceros donde va a ir el color


		PACKSSDW	XMM10, XMM11
		PACKUSWB	XMM10, XMM10
		PAND 		XMM10, XMM12;Borro el lugar del color
		XORPD 		XMM12, XMM12
		PINSRW 		XMM12, EAX, 4; posiciono el color 
		POR 		XMM10, XMM12; y lo inserto en el lugar que hice de ceros

		; ahora en la posicion 8 tengo que colocar el color que me  quedaba colgado de xmm12


		;Pasar a registro de salida
		MOVDQA		XMM0, XMM10

		LEA R15, [6 + PX_BLUR_ITER]  ; informo que levante 3 (por ahora 2 por eso 8 y no 9)
			
;---------------------------------------------------------------------------------
;----------------------------FIN FILTRO-------------------------------------------
;---------------------------------------------------------------------------------



			.escribir:
			MOV		R13, RSI		;dst
			ADD		R13, R12		;dst+indice_pixel
			MOVDQU	[R13], XMM0		;pasar a dst 


			.siguientePixel:
			DEC 	R15 	;resto el pixel que acabo de evaluar a la variable de procesamiento batch
			INC 	R11		; columna + 1


			;----Me fijo si hay mas columnas
			MOV 	R13, R14	;width
			DEC		R13			;width - 1
			CMP		R13, R11 	;columna == width - 1 ?
			JE		.finColumnas
			JMP		.cicloColumnas

		.finColumnas:
		XOR		R11, R11 	;reseteo contador 
		;----Me fijo si hay mas filas
		MOV 	R13, RCX
		DEC		R13
		CMP		R13, RBX
		JLE		.finFilas
		INC 	RBX
		JMP		.cicloFilas
		


	.finFilas:

	;recupero dato de iteracion
	POP 	R15
	INC 	R15
	XOR		RBX, RBX ;reseteo contador 
	;----me fijo si quedan iteraciones
	MOV 	R13, R8
	CMP		R13, R15
	JE		.fin
	

	;paso el src como dst
	MOV R13, RSI
	MOV RSI, RDI
	MOV RDI, R13


	;-----recupero valores para iteracion----------------------
	
	POP R10
	POP R9

	JMP		.cicloIteraciones


.fin:
;---Deshago el stackframe-----------
POP R10
POP R9

ret
;R14:	width
;RCX:	height