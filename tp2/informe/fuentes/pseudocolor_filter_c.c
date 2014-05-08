#define OFFSET_BLUE 0
#define OFFSET_GREEN 1
#define OFFSET_RED 2
#define TAMANIO_MAT_BYTES 3*width*height

void color_filter_c(unsigned char *src,
                    unsigned char *dst,
                    unsigned char rc,
                    unsigned char gc,
                    unsigned char bc,
                    int threshold,
                    int width,
                    int height)
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	int diffR;
	int diffG;
	int diffB;
	int distanciaCuad;
	unsigned short promedio;
	unsigned char promedioRes;
	threshold*=threshold;

	for(int index=0;index<TAMANIO_MAT_BYTES;index+=3){
		r = src[index + OFFSET_RED];
		g = src[index + OFFSET_GREEN];
		b = src[index + OFFSET_BLUE];
		
		diffR=r-rc;
		diffG=g-gc;
		diffB=b-bc;

		diffR *= diffR;
		diffG *= diffG;
		diffB *= diffB;

		distanciaCuad = (diffR+diffG+diffB);
		if(distanciaCuad>threshold){
			promedio = r+g+b;
			promedio /= 3;
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
