global color_filter_asm

%macro	processPixels 1;corrimiento en bytes

;.memLoad:
		;copio 16 bytes a XMM0
		MOVDQU XMM0, [RDI + R12 + %1]
		
		;copio XMM0 para reagrupar los pixeles segun componentes y
		;realizar el calculo de distancia y el filtro por threshold
		MOVDQA XMM1, XMM0
		MOVDQA XMM2, XMM0

		;"desempaqueto" de BGR,BGR,BGR,BGR(3 bytes por pixel) a 3 registros
		;conteniendo BLUE, GREEN, RED correspondientemente con padding de ceros en 3 bytes,
		;completando 4 dwords
		PSHUFB XMM0, XMM6;[MASK_BLUE]
		PSHUFB XMM1, XMM7;[MASK_GREEN]
		PSHUFB XMM2, XMM8;[MASK_RED]

		;resguardo XMM0, XMM1, XMM2 para aplicar el filtro propiamente dicho
		;usando la mascara que surja de comparar distCuad con thresholdCuad y su mascara negada
		MOVDQA XMM3, XMM0
		MOVDQA XMM4, XMM1
		MOVDQA XMM5, XMM2

;.diffCalc:
		;calculo las diferencias
		PSUBD XMM0, XMM15
		PSUBD XMM1, XMM14
		PSUBD XMM2, XMM13

		;elevo al cuadrado
		PMULLD XMM0, XMM0
		PMULLD XMM1, XMM1
		PMULLD XMM2, XMM2

		;ahora sumo las diferencias
		PADDD XMM0, XMM1
		PADDD XMM0, XMM2
		
		;ahora comparo con threshold al cuadrado
		;XMM1, XMM2 pueden ser reutilizados
;.thresholdCmp:
	
		;en XMM0 tengo la distancia al cuadrado, la convierto a float para comparar
		;TODO:lo quise hacer con PCMPGTD y negando con PXOR contra 0xFFFF... pero me volvi loco
		;debe ser mas rapido si comparo con enteros.me ahorro la conversion a float
		;y la comparacion de punto flotante.
		CVTDQ2PS XMM0, XMM0
		;copio el threshold(XMM12), para aplicar la mascara por lower than y not lower than
		;en el proximo ciclo, lo voy a necesitar nuevamente asi que lo copio
		MOVAPS XMM9, XMM12
		MOVAPS XMM10, XMM12
		CMPNLTPS XMM9, XMM0;aplico mascara por not lower than
		CMPLTPS XMM10, XMM0;threshold*threshold < distanciaCuad. aplico mascara por lower than
		;XMM0 puede ser reutilizado
		;tengo en XMM10, XMM9, como 4 dwords mascaras de 0xFFFF o 0x0000 segun sea true o false respectivamente
		;notar que como compare con mascaras excluyentes bit a bit el resultado de aplicar ands tambien lo sera

		;en XMM3, XMM4, XMM5 tengo los pixeles originales...agrupados por componente como 4 dwords
		;me copio los originales, dado que algunos quedaran en blanco y negro y otros no
		MOVDQA XMM0, XMM3
		MOVDQA XMM1, XMM4
		MOVDQA XMM2, XMM5

		;aplico mascara con AND para tener los que van en blanco y negro y los que quedan originales separados
		;segun las dos mascaras
		PAND XMM0, XMM10
		PAND XMM1, XMM10
		PAND XMM2, XMM10

		PAND XMM3, XMM9
		PAND XMM4, XMM9
		PAND XMM5, XMM9
;.blancoNegro:
		;paso a blanco y negro los que correspondan
		;sumo de a enteros
		PADDD XMM0, XMM1
		PADDD XMM0, XMM2
		;convierto de 4 dwords a 4 single floating point
		CVTDQ2PS XMM0, XMM0
		DIVPS XMM0, XMM11;XMM11 tiene 3, divido por 3
		;convierto a 4 dwords los 4 float con los px que estan en blanco y negro
		CVTPS2DQ XMM0, XMM0

;.pxCombine:
		;combino los pixeles filtrados con los ignorados
		;en XMM0 esta el blanco negro (igual para los 3 componentes)
		POR XMM3, XMM0
		POR XMM4, XMM0
		POR XMM5, XMM0
		;las mascaras se pueden obtener haciendo shift de la primera(blue reverse) pero bizarramente
		;es mas lento hacer mov xmm9, [MASK_BLUE_REVERSE] y hacer las otras 2 con shifts sobre registros
		;que hacer los 3 accesos a memoria...
		PSHUFB XMM3, [MASK_BLUE_REVERSE]
		PSHUFB XMM4, [MASK_GREEN_REVERSE]
		PSHUFB XMM5, [MASK_RED_REVERSE]

		;combino los pixeles para tenerlos en BGRBGRBGR...
		POR XMM3, XMM4
		POR XMM3, XMM5
;.dstCpy:
		;copio a destino
		MOVDQU [RSI + R12 + %1], XMM3
%endmacro

section .data

ALIGN 16
MASK_BLUE: DB 0, 255, 255, 255, 3, 255, 255, 255, 6, 255, 255, 255, 9, 255, 255, 255
ALIGN 16
MASK_GREEN: DB 1, 255, 255, 255, 4, 255, 255, 255, 7, 255, 255, 255, 10, 255, 255, 255
ALIGN 16
MASK_RED: DB 2, 255, 255, 255, 5, 255, 255, 255, 8, 255, 255, 255, 11, 255, 255, 255

ALIGN 16
MASK_BLUE_REVERSE: DB 0, 255, 255, 4, 255, 255, 8, 255, 255, 12, 255, 255, 255, 255, 255, 255
ALIGN 16
MASK_GREEN_REVERSE: DB 255, 0, 255, 255, 4, 255, 255, 8, 255, 255, 12, 255, 255, 255, 255, 255
ALIGN 16
MASK_RED_REVERSE: DB 255, 255, 0, 255, 255, 4, 255, 255, 8, 255, 255, 12, 255, 255, 255, 255

ALIGN 16
THREE: DB 3

section .text
;void color_filter_asm(unsigned char *src,
;                    unsigned char *dst,
;                    unsigned char rc,
;                    unsigned char gc,
;                    unsigned char bc,
;                    int threshold,
;                    int width,
;                    int height);
color_filter_asm:
;en RDI viene src
;en RSI viene dst
;en la parte baja(8 bits) de RDX(DL) viene rc
;en la parte baja(8 bits) de RCX(CL) viene gc
;en la parte baja(8 bits) de R8(R8b) viene bc
;en la parte baja(32 bits) de R9(R9d) viene threshold
;1 pop y tengo width(32 bits)
;2 pop y tengo height(32 bits)

PUSH RBP
MOV RBP, RSP
;la pila esta asi de tope a base
;RBP|RIP|width|height
;RSP apunta a donde esta pusheado RBP, 
;se accede a las demas asi: [RSP]=RBP; [RSP+8]=RIP; [RSP+16]=width; [RSP+24]=height, etc...
PUSH R12;index ciclo
PUSH R13;limit ciclo
PUSH R14;width
PUSH R15;height
PUSH RBX
SUB RSP, 8;para alinear
;ahora movi todo el rsp al pushear, pero en rbp tengo el rsp de como venia la pila al toque de la llamada.

;width esta apuntado por RBP+16(RBP, RIP estan adelante)
;height esta apuntado por RBP+24(RBP, RIP, width estan adelante)
;limpio R14 y R15
XOR R14, R14
XOR R15, R15
MOV dword R14d, [RBP+16];copio width;
MOV dword R15d, [RBP+24];copio height;
;notar que los paso a var de 64 bits!

MOV R12, 0;inicializo index
MOV R13, R14;copio width
IMUL R13, R15;multiplico por height
IMUL R13, 3;ahora tengo 3*width*height (osea limit)
;elevo al cuadrado threshold
IMUL R9d, R9d;elevo al cuadrado

;genero registros XMM con mascaras de rc, gc, bc
;dword con 4 veces rc
PXOR XMM13, XMM13
PINSRB XMM13, EDX, 0;inserta el byte mas bajo del registros de proposito general en el indice indicado
PSHUFD XMM13, XMM13, 00000000

;dword con 4 veces gc
PXOR XMM14, XMM14
PINSRB XMM14, ECX, 0;inserta el byte mas bajo del registros de proposito general en el indice indicado
PSHUFD XMM14, XMM14, 00000000

;dword con 4 veces bc
PXOR XMM15, XMM15
PINSRB XMM15, r8d, 0;inserta el byte mas bajo del registros de proposito general en el indice indicado
PSHUFD XMM15, XMM15, 00000000

;mascara threshold*threshold como 4 floats packed
PINSRD XMM12, R9d, 0
PSHUFD XMM12, XMM12, 0
;convierto a float empaquetado
CVTDQ2PS XMM12, XMM12

;constante 3 como 4 floats packed
PINSRD XMM11, [THREE], 0
PSHUFD XMM11, XMM11, 0
;convierto a float empaquetado
CVTDQ2PS XMM11, XMM11

;copio mascaras de memoria a registros
MOVDQA XMM6, [MASK_BLUE]
MOVDQA XMM7, [MASK_GREEN]
MOVDQA XMM8, [MASK_RED]

.ciclo:	CMP R12, R13
		JGE .finCiclo
		processPixels 0
	
		processPixels 12

		processPixels 24

		processPixels 36

		processPixels 48

		processPixels 60

		processPixels 72

		processPixels 84

		processPixels 96

		processPixels 108

		processPixels 120

		processPixels 132

		processPixels 144

		processPixels 156

		processPixels 168

		processPixels 180

.incrPtr:ADD R12, 192;12 del ciclo original +12*cantDeUnrolls
		;copio de a 16 bytes(4 pixeles, ignoro los ultimos 4 bytes del registro)
		;entonces debo procesar nuevamente en 12 bytes siguientes(proceso de a 4 px (3*4bytes) por ciclo)
		JMP .ciclo
.finCiclo:

ADD RSP, 8;contrarestar el SUB para alinear
POP RBX
POP R15
POP R14
POP R13
POP R12
POP RBP
    ret

