void aplicarFiltro(int matriz[],
				   int indice_pixel,
				   unsigned char *src,
				   unsigned char *dst,
				   int width){

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