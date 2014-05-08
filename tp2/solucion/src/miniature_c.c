/*  topPlane:
        Numero entre 0 y 1 que representa el porcentaje de imagen desde el cual
        va a comenzar la primera iteración de blur (habia arriba)

    bottomPlane:
        Numero entre 0 y 1 que representa el porcentaje de imagen desde el cual
        va a comenzar la primera iteración de blur (hacia abajo)

    iters:
        Cantidad de iteraciones. Por cada iteración se reduce el tamaño de
        ventana que deben blurear, con el fin de generar un blur más intenso
        a medida que se aleja de la fila centro de la imagen.
*/

#define PIXEL 3
#define OFFSET_RED 2
#define OFFSET_GREEN 1
#define OFFSET_BLUE 0

#define LADO_BLUR 5

void aplicarFiltro(int matriz[], int index_pixel, unsigned char *src, unsigned char *dst, int width);
void miniature_c(
                unsigned char *src,
                unsigned char *dst,
                int width, int height,
                float topPlane, float bottomPlane,
                int iters) {

	int fila = 0;
	int columna = 0;
	int matriz[5] = {1,5,18, 32, 64};//Coeficientes multiplicados por 100
	
	int indice_pixel = 0;

	//cantidad de filas totales en cada banda
	int cantidad_filas_top = (int)((float)(height) * topPlane); 
	int cantidad_filas_bottom =  height - (int)((float)(height) * bottomPlane);//las que sobran (asi esta en el enunciado)
	
	for(int iteracion = 0; iteracion < iters; iteracion++){

		//filas a evaluar segun la cuenta de la especificacion
		int cantidad_filas_top_evaluadas = cantidad_filas_top - (int)((iteracion * cantidad_filas_top) / iters ) ;
		int cantidad_filas_bottom_evaluadas = cantidad_filas_bottom - (int)((iteracion * cantidad_filas_bottom) / iters );

		//redondeo para el mas cercano (especificado implicitamente en los tests)
		if ((iteracion * cantidad_filas_top) % iters > 0){
			cantidad_filas_top_evaluadas = cantidad_filas_top -1 - (int)((iteracion * cantidad_filas_top) / iters ) ;
		}
		/*if ((iteracion * cantidad_filas_top) % iters > iters/2){
			cantidad_filas_bottom_evaluadas = cantidad_filas_bottom -1 - (int)((iteracion * cantidad_filas_bottom) / iters );

		}*/



		//ubicacion de las fiilas que limitan las bandas
		int ultima_fila_topPlane = cantidad_filas_top_evaluadas; //empieza en la cero
		int primera_fila_bottomPlane = height - cantidad_filas_bottom_evaluadas;


		for (fila = 0; fila < height; fila++){
			for(columna = 0; columna < width; columna++){

				indice_pixel = columna * PIXEL + fila * width * PIXEL;

				//fila inicio pasa a ser la 3ra por especificacion implicita de tests
				int no_esBorde = fila > 2 && fila < (height-2) && columna > 1 && columna < (width - 2);
				int esta_dentroFrames = fila <= ultima_fila_topPlane || fila >= primera_fila_bottomPlane;

				if(no_esBorde && esta_dentroFrames){

					aplicarFiltro(matriz, indice_pixel, src, dst, width);

				}else{
					dst[indice_pixel+OFFSET_BLUE] = src[indice_pixel+OFFSET_BLUE];
					dst[indice_pixel+OFFSET_GREEN] = src[indice_pixel+OFFSET_GREEN];
					dst[indice_pixel+OFFSET_RED] = src[indice_pixel+OFFSET_RED];
				}//end if
			}//end for columnas
		}//end for filas

		unsigned char *src_aux = src; 
		src = (unsigned char*)dst;
		dst = (unsigned char*)src_aux;
	}//end for iteraciones
}

void aplicarFiltro(int matriz[], int indice_pixel, unsigned char *src, unsigned char *dst, int width){

	int r=0;
	int g=0;
	int b=0;



	int fila = 0;
	int columna = 0;

	int offset_area_blur = indice_pixel - 2 * PIXEL - 2 * width * PIXEL;


	
	for (fila = 0; fila < LADO_BLUR; fila++){

		for (columna = 0; columna <LADO_BLUR; columna++){

			int indice_pixel_contorno =  offset_area_blur +  (columna * PIXEL + fila * width * PIXEL);
			
			int r_aux = (int) src[indice_pixel_contorno+OFFSET_RED];
			int g_aux = (int) src[indice_pixel_contorno+OFFSET_GREEN];
			int b_aux = (int) src[indice_pixel_contorno+OFFSET_BLUE];
			int coef = 0;


			//Aplicacion de coeficientes de la matriz
			if (fila == 0 || fila == 4){

				if(columna == 0 || columna == 4){

					coef =  matriz[0];//0.01

				}else if (columna == 1 || columna == 3){

					coef =  matriz[1];//0.05
				
				}else{

					coef = matriz[2];//0.18
				}

			}else if (fila == 1 || fila == 3){

				if(columna == 0 || columna == 4){

					coef =  matriz[1];//0.05

				}else if (columna == 1 || columna == 3){

					coef =  matriz[3];//0.32
				
				}else{

					coef = matriz[4];//0.64
				}
			}else{

				if(columna == 0 || columna == 4){

					coef =  matriz[2];//0.18

				}else if (columna == 1 || columna == 3){

					coef =  matriz[4];//0.64
				
				}else{

					coef = 100; //pixel objetivo;
				}

			}//fin coeficientes matriz

			r += r_aux * coef;
			g += g_aux * coef;
			b += b_aux * coef;
		}
	}

	//--------------normalizo valores---------------//

	r = (r/100)/6;
	g = (g/100)/6;
	b = (b/100)/6;
	//---------------guaardo en destino-------------//
	dst[indice_pixel+OFFSET_BLUE] = (unsigned char) b;
	dst[indice_pixel+OFFSET_GREEN] = (unsigned char) g;
	dst[indice_pixel+OFFSET_RED] = (unsigned char) r;

	
}
