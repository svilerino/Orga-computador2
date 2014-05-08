global decode_asm

extern memset

%macro merge4BytesXMM5CharInOutput 5;pos0,pos1;pos2;pos3;registerOutput
		PEXTRB R8, XMM5, %1;levanto el primer byte de XMM5 en R8
		PEXTRB R9, XMM5, %2;levanto el segundo byte de XMM5 en R9
		PEXTRB R10, XMM5, %3;levanto el tercer byte de XMM5 en R10
		PEXTRB R11, XMM5, %4;levanto el cuarto byte de XMM5 en R11
		SHL R8, 6;corro 6 lugares el primer byte
		SHL R9, 4;corro 4 lugares el primer byte
		SHL R10, 2;corro 2 lugares el primer byte
		;SHL R11, 0 no hace falta porque seria un shift 0 de lugares

		;PEXTRB me hace zero fill de las partes altas de los Rxx
		;hacer un or entre R8,R9,R10,R11 en output register
		XOR %5, %5;limpio el registro output
		OR %5, R8
		OR %5, R9
		OR %5, R10
		OR %5, R11

		;aca tengo en el registro %5 zerofill y en los 8 bits mas bajos el resultado
%endmacro

section .data
ALIGN 16
CODE_FILTER_MASK: DB 3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3;00000011(16 veces)
ALIGN 16
OP_FILTER_MASK: DB 12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12;(00001100 16 veces)
ALIGN 16
DQ0_MASK: DB 00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
ALIGN 16
DQ1_MASK: DB 01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01
ALIGN 16
DQ2_MASK: DB 02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02
ALIGN 16
DQ3_MASK: DB 03,03,03,03,03,03,03,03,03,03,03,03,03,03,03,03

section .text
;Asumiendo que size es la cantidad de chars del mensaje.
;Tambien voy a asumir cant_bytes_mat_message < tamanio_mat_bytes
decode_asm:
;en RDI viene src
;en RSI viene code
;en EDX viene size
;en ECX viene width
;en R8d viene height	
	PUSH R12;R12 va a tener el contador del ciclo
	PUSH R13;R13 va a tener el limite del ciclo
	PUSH R14;R14 va a tener el indice del mensaje
	PUSH R15;ciclo interno
	PUSH RBX;ciclo interno
;comienza codigo

;tengo que recorrer 4*size bytes de la matriz
;inicializo los indices
MOV R13, 0;inicializo todo el registro en 0
MOV R13d, EDX; copio la parte baja al registro(size)
SHL R13, 2;multiplico por 4, dejo en R13 4*size como limite para iterar
MOV R12, 0;inicializo el contador en 0
MOV R14, 0;inicializo el indice del mensaje salida en 0
;inicializo los XMM con mascaras y valores iniciales
MOVDQA XMM10, [DQ0_MASK]
MOVDQA XMM11, [DQ1_MASK]
MOVDQA XMM12, [DQ2_MASK]
MOVDQA XMM13, [DQ3_MASK]
MOVDQA XMM14, [CODE_FILTER_MASK]
MOVDQA XMM15, [OP_FILTER_MASK]

;memseteo con nulls el buffer de salida
;convencion C, la pila esta alineada
PUSH RDI
PUSH RSI
MOV RDI, RSI;ptr size
MOV ESI, 0;'\0', int size
;en EDX ya esta size que es unsigned int(int size)
CALL memset
POP RSI
POP RDI

;ciclo principal
ciclo:	CMP R12, R13
		JGE finCiclo

		MOVDQU XMM0, [RDI + R12]; levanto 16 bytes de la matriz indexada por SRC+INDEX = RDI+R12, cada 4 bytes tengo un char efectivo
		MOVDQU XMM1, XMM0;me copio a XMM1 los 16 bytes levantados de la matriz sin modificar		
		
		;ejecuto ANDs empaquetados con mascaras para obtener code y op		
		PAND XMM1, XMM14
		;ahora tengo en XMM1 todos los code empaquetados en byte
		;hago el and para los op, notar que me van a quedar "corridos" 2 lugares porque es un and 00001100
		PAND XMM0, XMM15
		;si hago un shift a derecha logico no empaquetado, es lo mismo que hacerlo empaquetado a byte en este caso
		PSRLQ XMM0, 2;shift empaquetado, lo hago porque el shift dqword shifea BYTES en vez de bits FUCK INTEL LOGIC.
		;ahora tengo en XMM0 todos los op empaquetados en byte

		;ya separe los op y code en XMM0 y XMM1 respectivamente en formato 000000xx empaquetado en 16 bytes

		;ahora tengo que ver que hacer con los bits code segun los op
		;hago 4 comparaciones empaquetadas de a byte por igualdad con mascaras de los op hardcodeadas
		;en 4 registros XMM

		;copio 3 veces los op para resguardarlos ya que al aplicar mascaras a XMM0 pierdo informacion 
		MOVDQU XMM2, XMM0
		MOVDQU XMM3, XMM0
		MOVDQU XMM4, XMM0

		PCMPEQB XMM0, XMM10; [DQ0_MASK]
		PCMPEQB XMM2, XMM11; [DQ1_MASK]
		PCMPEQB XMM3, XMM12; [DQ2_MASK]
		PCMPEQB XMM4, XMM13; [DQ3_MASK]
		;ahora tengo en XMM0, XMM2, XMM3, XMM4 las mascaras de 0x00 o 0xFF segun sean los op 00,01,10,11 respectivamente

		; me copio a XMM5 los codes originales, XMM5 es auxiliar
		MOVDQU XMM5, XMM1
		MOVDQU XMM7, XMM1;XMM7 auxiliar
		MOVDQU XMM8, XMM1;XMM8 auxiliar
		MOVDQU XMM6, XMM1;XMM6 auxiliar
		;los que quedaron "marcados" en XMM0 quedan igual
		;por lo tanto no hacemos nada y quedan iguales en XMM1

		;los que quedaron "marcados" en XMM2 se suma 1 mod 4
		PADDB XMM7, XMM11;[DQ1_MASK];sumo 1 empaquetado a byte
		PAND XMM7, XMM13;[DQ3_MASK]; sumar y hacer esta mascara equivale a modulo 4		
		;ahora tengo en XMM2 los code+1 mod 4

		;los que quedaron "marcados" en XMM3 se resta 1 mod 4(ojo que si es negativo clavas 3 hardcode)
		;notar que por propiedades de congruencia (k-1)%4 = (k+3)%4
		PADDB XMM8, XMM13;[DQ3_MASK];sumo 3 empaquetado a byte
		PAND XMM8, XMM13;[DQ3_MASK]; sumar y hacer esta mascara equivale a modulo 4		
		;ahora tengo en XMM3 los code-1 mod 4

		;los que quedaron "marcados" en XMM4 se niega el byte y se limpian los 6 bits mas altos a 0
		PANDN XMM6, XMM13;[DQ3_MASK];PNOT(no existe pero seria un not bit a bit) XMM6 + PAND XMM6, 00000011		
		;ahora tengo en XMM4 los code negados

		;secciono los resultados por las mascaras antes de combinar
		PAND XMM5, XMM0;aplico la mascara para filtrar los que deben aplicar con esto
		PAND XMM7, XMM2;aplico la mascara para filtrar los que deben aplicar con esto			
		PAND XMM8, XMM3;aplico la mascara para filtrar los que deben aplicar con esto	
		PAND XMM6, XMM4;aplico la mascara para filtrar los que deben aplicar con esto

		;combino las 4 posibilidades que habia
		;notar que son excluyentes bit a bit los 4 registros con los code finales
		POR XMM5, XMM7
		POR XMM5, XMM8
		POR XMM5, XMM6

		;ahora tengo que mergear los 16 bytes en XMM1 que solo tienen los 2 bits mas bajos(cada byte empaquetado) validos
		;en 4 bytes ascii en XMM1

		merge4BytesXMM5CharInOutput 3, 2, 1, 0, RAX
		merge4BytesXMM5CharInOutput 7, 6, 5, 4, RBX
		merge4BytesXMM5CharInOutput 11, 10, 9, 8, RCX
		merge4BytesXMM5CharInOutput 15, 14, 13, 12, RDX

		MOV byte [RSI+R14], AL;copio el char a la salida(base+desplazamiento)			
		MOV byte [RSI+R14+1], BL;copio el char a la salida(base+desplazamiento)			
		MOV byte [RSI+R14+2], CL;copio el char a la salida(base+desplazamiento)			
		MOV byte [RSI+R14+3], DL;copio el char a la salida(base+desplazamiento)			

		ADD R14, 4;incremento indice j de salida
incrPtr:ADD R12, 16
		JMP ciclo
finCiclo: 

	DEC R14;al final del ciclo estaba pasado en uno y sigsev
	MOV byte [RSI+R14], 0;pongo el ultimo byte(base msj + ultimo indice char) en null
	POP RBX
	POP R15
	POP R14
	POP R13
	POP R12
    ret

;intento de implementar software pipelining con desenrrollado de ciclos.
;		MOVDQU XMM0, [RDI + R12]
;		MOVDQU XMM2, [RDI + R12+16]
;
;		MOVDQA XMM1, XMM0
;		MOVDQA XMM3, XMM2
;
;		;obtengo los codes
;		PAND XMM1, XMM14
;		PAND XMM3, XMM14
;
;		;obtengo los ops
;		PAND XMM0, XMM15
;		PAND XMM2, XMM15
;		PSRLQ XMM2, 2
;		PSRLQ XMM0, 2
;
;		;copio 4 veces cada serie de ops
;		MOVDQA XMM4, XMM1
;		MOVDQA XMM5, XMM1
;		MOVDQA XMM6, XMM1
;
;		MOVDQA XMM7, XMM3
;		MOVDQA XMM8, XMM3
;		MOVDQA XMM9, XMM3
;
;		;aplico las mascaras en las series de codes
;		PCMPEQB XMM0, XMM10; [DQ0_MASK]
;		PCMPEQB XMM4, XMM11; [DQ1_MASK]
;		PCMPEQB XMM5, XMM12; [DQ2_MASK]
;		PCMPEQB XMM6, XMM13; [DQ3_MASK]
;
;		PCMPEQB XMM2, XMM10; [DQ0_MASK]
;		PCMPEQB XMM7, XMM11; [DQ1_MASK]
;		PCMPEQB XMM8, XMM12; [DQ2_MASK]
;		PCMPEQB XMM9, XMM13; [DQ3_MASK]
;		;ahora tengo las mascaras de 0x00 o 0xFF segun sean los op 00,01,10,11 respectivamente
;
;		no me alcanzan los registros para probar sotware pipelining con desenrrollado de ciclo x2