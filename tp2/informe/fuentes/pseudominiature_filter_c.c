
void miniature_c(
                unsigned char *src,
                unsigned char *dst,
                int width,
                int height,
                float topPlane,
                float bottomPlane,
                int iters)
{

	int matriz[5] = {1,5,18, 32, 64};

	for(iteracion = 0 to iters-1){

		for (fila = 0 to height-1){

			for(columna = 0 to width - 1){

				indice_pixel = calcularPosicionReal(src, fila, columna);

				bool condicion1 = no_esBorde(src, indice_pixel);
				bool condicion2 = esta_dentroFrames();

				if( condicion1 && condicion2 ){

					aplicarFiltro(src, dst, matriz, indice_pixel);

				}else{
					
					copiarIgual(src, dst, indice_pixel)

				}//if
			}// columnas for
		}// filas for

		intercambiar(src, dst);

	}// iteraciones for

}// funcion


