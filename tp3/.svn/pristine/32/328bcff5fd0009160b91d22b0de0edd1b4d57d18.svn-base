%define formatoEscritura 0x07
ORG 0x1200;indica al compilador que los saltos a etiquetas deben sumar este offset

BITS 16;modo de programacion en 16 bits(compila en 16 bits)

    cli;clear interrupt flag
;    xchg bx,bx ;breakpoint bochs

;---------------------- ESCRIBIMOS UN MENSAJE EN MODO REAL -------------------;
;pongo en es(segmento auxiliar), la posicion a la memoria 0xB800, cuando se haga el direccionamiento con shift
;va a quedar bien B8000h direccionando a la memoria de video
mov ax, 0B800h
mov es, ax
;inicializo el indice para escribir en memoria en 0
mov bx, 0
mov ah, formatoEscritura
cicloReal:
	mov al, [mensaje_real+bx]
	cmp byte al, 0
	je finCicloReal
	
	sal bx, 1
	mov word [es:bx], ax
	sar bx, 1

	inc bx
jmp cicloReal
finCicloReal:
;descomentar esto del jump para ver lo que imprime, sino modo protegido pisa la memo de video.
;jmp $
;--------------------------------- CARGAMOS GDT ------------------------------;
	LGDT [gdt_desc];cargo posicion de la gdt en el registro
;------------------------------- ACTIVAMOS BIT PE ----------------------------;
	MOV EAX, CR0;levanto registro CR0 para pasar a modo protegido
	OR EAX, 1;hago un or con una mascara de 0...1 para setear el bit de modo protegido
	MOV CR0, EAX
	jmp 0x8:protected_mode; saltamos a modo protegido, modificamos el cs con un jump y la eip(program counter)
	;0x8 = 1000 bin , notar que es la forma en la que indexo la gdt(los primeros 3 bits son flags)
	;{index:1 | gdt/ldt: 0 | rpl: 00}

BITS 32;modo de programacion en 32 bits(compila en 32 bits)
protected_mode:
;cargo los selectores de segmento
xor eax, eax
mov ax, 10000b;{index:2 | gdt/ldt: 0 | rpl: 00}
mov ds, ax;tiene sentido, pues el 3 elemento(el primero es nulo) es el segmento de datos que declaramos en la gdt
mov es, ax;TODO: ?
mov gs, ax;TODO: ?
mov ax, 11000b;{index:3 | gdt/ldt: 0 | rpl: 00} 
mov fs, ax;utilizamos el selector de segmento auxiliar para usar memoria de video

;----------------- ESCRIBIMOS UN MENSAJE EN MODO PROTEGIDO -------------------;
mov ah, formatoEscritura
mov ebx, 0;indice
cicloProtegido: 	
		cmp byte [mensaje_protegido+ebx], 0;cuando encuentre el caracter nulo, corta el ciclProtegidoo 
		je finCicloProtegido
		mov al, [mensaje_protegido+ebx]
		mov word [fs:ebx*2], ax
	inc ebx
jmp cicloProtegido
finCicloProtegido:

;----------------- ACA TERMINA NUESTRO CODIGO 'LO COLGAMOS' ------------------;
    jmp $; esto vendria a ser un while(1) ? salta al comienzo de esta linea eternamente


;=================== A PARTIR DE ESTE PUNTO PEGO LA GDT ======================;
;supongo que es para que no se ejecute(como no hay entrypoint)

mensaje_real: db "estoy en modo real", 0
mensaje_protegido: db "estoy en modo protegido", 0

gdt:
;el primer descriptor de segmento es nulo
;cada descriptor ocupa 8 bytes, so piola.
dd 0x0000
dd 0x0000

;segmento de codigo de nivel 0
;sc0:	dd 0x00003FFF
;		dd 0x00C09A00
sc0: 
	;segment limit 16 bits
	db 11111111b;0xFF
	db 11110011b;0x3F
	;base address 16 bits
	db 00000000b;0x00
	db 00000000b;0x00
	;base 23:16
	db 00000000b;0x00
	;type: 1001 ex. code accessed
	;S: 1 code or data
	;DPL: 01 descriptor privilege level TODO: no va 0 aca?
	;P segment present: 0
	db 10011010b;0x9A
	;segm limit 19:16 => 1100
	;available from system software:0
	;l(64 bit segm): 0
	;d/b(16/32 bits segm): 0
	;granularity: 0
	db 11000000b;0xC0
	;base 31:24
	db 00000000b;0x00

;segmento de datos de nivel 0
;sd0:	dd 0x00003FFF
;		dd 0x00C09200
;lo mismo que la sc0 pero cambian los bits del tipo a data read write
sd0:
	db 11111111b;0xFF
	db 11110011b;0x3F
	db 00000000b;0x00
	db 00000000b;0x00

	db 00000000b;0x00
	db 10010010b;0x92
	db 11000000b;0xC0
	db 00000000b;0x00

;segmento de datos de nivel 0 que comienza en b8000 lo cual es memoria de video
;video: 	dd 0x80000FBF
;		dd 0x0040920B
video: 	
		dw 0x0FBF;segm limit
		dw 0x8000;base address
		db 0x0B;base 23:16 => completa base en 0xB8000
		;type: 0010 data read write
		;S: 1 code or data
		;DPL: 01 descriptor privilege level TODO: no va 0 aca?
		;P segment present: 0
		db 10010010b;0x92
		;segment limit 19:16 => 0100
		;available from system software:0
		;l(64 bit segm): 0
		;d/b(16/32 bits segm): 0
		;granularity: 0
		db 01000000b;0x40
		;base 31:24 => 0x00
		db 00000000b;0x00

gdt_desc: 	dw $-gdt ;tamaño de tabla utilizando simbolo $ que indice memoria, haciendo diferencia contra gdt tenemos la cant de bytes
			dd gdt 	 ;direccion de la tabla
;gdt_desc denota lo siguiente | 2 bytes tamaño de la tabla | 4 bytes direccion de la tabla |