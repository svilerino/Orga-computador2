BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; habilitar A20    
    call habilitar_A20

    ;desaparecer cursor en pantalla
    mov BL, 0
    dec BL
    mov BH, 0
    dec BH
    set_cursor

    ; cargar la GDT    
    lgdt [GDT_DESC]
   
    mov EAX, CR0
    OR EAX, 1
    mov CR0, EAX
    ; pasar a modo protegido
    jmp 0x90:protected_mode
   
BITS 32;modo de programacion en 32 bits(compila en 32 bits)
protected_mode:
    ;cargo los selectores de segmento
    xor eax, eax
    mov ax, 10110000b
    mov fs, ax
    mov ax, 10011000b
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov ss, ax
    ;inicializo la pila
    mov esp, 0x27000
    mov ebp, esp