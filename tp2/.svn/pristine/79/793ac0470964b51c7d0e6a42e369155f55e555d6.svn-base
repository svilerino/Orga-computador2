#define OFFSET_BLUE 0
#define OFFSET_GREEN 1
#define OFFSET_RED 2
#define TAMANIO_MAT_BYTES 3*width*height
//objdump -r -d -S -M intel-mnemonic color_filter_c.o
//gcc -c -O1 -std=c99 color_filter_c.c -o color_filter_c.o

void color_filter_c(unsigned char *src,
                    unsigned char *dst,
                    unsigned char rc,
                    unsigned char gc,
                    unsigned char bc,
                    int threshold,
                    int width,
                    int height)
{
	threshold*=threshold;
	//elevo al cuadrado threshold afuera del for
	//declaro var. aux afuera del for
	unsigned char r;
	unsigned char g;
	unsigned char b;
	int diffR;
	int diffG;
	int diffB;
	int distanciaCuad;
	unsigned short promedio;
	unsigned char promedioRes;

	for(int index=0;index<TAMANIO_MAT_BYTES;index+=3){
		//levanto los datos de memoria una sola vez a var aux.
		r = src[index + OFFSET_RED];
		g = src[index + OFFSET_GREEN];
		b = src[index + OFFSET_BLUE];
		
		//sean a,b dos chars (bytes) sin signo, su diferencia esta en el rango [-255, 255],
		//con lo cual necesitamos realizarla en un signed short(word) para soportar este rango
		//usamos int porque aca tambien calculamos el cuadrado, y necesitamos el signo para la diff mencionada arriba
		//y el rango de int para la diffCuad maxima que es 255*255 = 65535, valor que entra en unsigned short
		//pero perdemos el signo para la diff, redondeando, usamos signed int(dword)
		diffR=r-rc;
		diffG=g-gc;
		diffB=b-bc;
		//elevo al cuadrado las diferencias, a lo sumo el resultado va a ser 65535,
		//esto entra en unsigned (short)word, pero necesitamos el signo para las diferencias anteriores.
		diffR *= diffR;
		diffG *= diffG;
		diffB *= diffB;
		//un int(dword) para almacenar el resultado es 3*65535 como maximo, lo cual entra en int
		distanciaCuad = (diffR+diffG+diffB);
		//me fijo si se aplica el filtro o no(threshold esta elevado 
		//al cuadrado en la linea antes del for)
		if(distanciaCuad>threshold){
			//variable de tipo unsigned short(word) para soportar la suma maxima antes de dividir.				
			//entra en unsigned short porque a lo sumo es 3*255 la suma
			promedio = r+g+b;
			promedio /= 3;
			//usando que: Sea A=[a1,a2,...an], luego vale que prom(A) <= Max(A)
			//luego podemos asegurar que el resultado del promedio esta acotado por algo que entra en byte
			//dado que los 3 sumandos son byte(unsigned char)
			//de esta forma podemos castear a unsigned char de forma segura y copiar el resultado a destino
			promedioRes = (unsigned char)promedio;							
			dst[index + OFFSET_RED] = promedioRes;
			dst[index + OFFSET_GREEN] = promedioRes;
			dst[index + OFFSET_BLUE] = promedioRes;
		}else{
			dst[index + OFFSET_RED] = r;
			dst[index + OFFSET_GREEN] = g;
			dst[index + OFFSET_BLUE] = b;
		}
	}
}

/*
 * Mejoras:
 * eliminacion de la modularizacion de dist
 * eliminacion de calculo de raiz usando elevacion al cuadrado con threshold
 * fix de for limites matriz
 * adaptacion a un solo for para tratar matriz como vector, mas facil para asm
 * elevacion al cuadrado de threshold afuera del for
 * declaracion de var auxiliares afuera del for
 */

