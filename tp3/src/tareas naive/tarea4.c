/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "colors.h"
#include "defines.h"
#include "game.h"
#include "syscall.h"
#include "i386.h"

#define BANDERA_BUFFER  0x40001000

/*
 * Estructura de la tarea :
 *
 *  0x40000000 - 0x40001000 : codigo tarea          (   5 KB)
 *  0x40001000 - 0x40001400 : area de banderas      (   1 KB)
 *  0x40001400 - 0x40001c00 : pila tarea            (   2 KB)
 *  0x40001c00 - 0x40001ffc : pila bandera          (1020 B )
 *  0x40001ffc - 0x40002000 : dir. funcion bandera  (   4 B )
 */

/* Estructura de para acceder a memoria de video */
typedef struct {
    unsigned char c;
    unsigned char a;
} ca;

unsigned char * bandera();

// global var
#define GLOBAL_START 0x40000000  
#define var_B GLOBAL_START+0x2000-0x200

void task() {
    /* Tarea 4 */
    unsigned int i = 0;
    while(1) {
//        for(i=0x00352000;i<0x0035A000;i+=0x1000){
//            if(i==0x1000){
//                breakpoint();//ALTISIMO YAO
//            }
//            syscall_navegar(i, i + 0x1000);
//        }
//        for(i=0;i<10*0x1000;i+=0x1000){
//            syscall_navegar(0x00352000 + i, 0x00353000 + i);
//        }
        if(i==1){
            i=0;
            syscall_navegar(0x00252000,0x00253000);
        }else{
            i=1;
            syscall_navegar(0x00182000,0x00183000);
        }
    };
}

unsigned char * bandera() {
    ca (*buffer)[10] = (ca (*)[10]) (BANDERA_BUFFER);
    int *b = (int*)(var_B);
    unsigned int fil;
    unsigned int col;
    if (*b == 1) *b = 0; else *b = 1;
    for (fil = 0; fil < 5; fil++) {
        for (col = 0; col < 10; col++) {
            buffer[fil][col].c = 220 ;
            if( *b == 0 )
                buffer[fil][col].a = C_BG_RED | C_FG_LIGHT_RED;
            else
                buffer[fil][col].a = C_BG_CYAN | C_FG_LIGHT_CYAN;
        }
    }
    syscall_bandera_fin((unsigned int) buffer);
    /* Para que el compilador no tire warning... */
    return 0;
}
